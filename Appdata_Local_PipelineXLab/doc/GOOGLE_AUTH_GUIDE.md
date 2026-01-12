# Google 인증 기반 회원 시스템 구현 가이드 (SQLAlchemy 2.0 Async)

## 개요

기존 SQLAlchemy 2.0 (Async) 기반 데이터베이스에 Google OAuth 2.0 회원 시스템을 통합합니다.
플로우, 세션과 함께 회원 정보를 단일 DB에서 관리합니다.

---

## 아키텍처

```
┌─────────────────────────────────────────────────────┐
│                   PostgreSQL                         │
├─────────────────────────────────────────────────────┤
│  기존 테이블              │  회원 테이블 (신규)      │
│  ─────────────            │  ─────────────           │
│  flows                    │  users                   │
│  sessions                 │  user_tokens             │
│  nodes                    │  activity_logs           │
│  components               │  tier_configs            │
└─────────────────────────────────────────────────────┘
                    ↑
    ┌───────────────┴───────────────┐
    │                               │
[Launcher App]              [Homepage]
```

---

## 1. SQLAlchemy 모델 정의

### 1.1 회원 등급 Enum

```python
# models/enums.py
import enum

class TierType(str, enum.Enum):
    FREE = "free"
    BASIC = "basic"
    PRO = "pro"
    ENTERPRISE = "enterprise"
```

### 1.2 Users 모델

```python
# models/user.py
from sqlalchemy import Column, String, DateTime, Boolean, Enum, Integer, Text
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func
from .base import Base
from .enums import TierType
import uuid

class User(Base):
    __tablename__ = "users"
    
    id = Column(String(36), primary_key=True, default=lambda: str(uuid.uuid4()))
    google_id = Column(String(255), unique=True, nullable=False, index=True)
    email = Column(String(255), unique=True, nullable=False, index=True)
    name = Column(String(255))
    profile_image = Column(Text)
    
    # 회원 등급
    tier = Column(Enum(TierType), default=TierType.FREE)
    tier_expires_at = Column(DateTime(timezone=True), nullable=True)
    
    # API 사용량
    api_calls_today = Column(Integer, default=0)
    api_calls_month = Column(Integer, default=0)
    last_api_call_at = Column(DateTime(timezone=True), nullable=True)
    
    # 메타데이터
    created_at = Column(DateTime(timezone=True), server_default=func.now())
    updated_at = Column(DateTime(timezone=True), onupdate=func.now())
    last_login_at = Column(DateTime(timezone=True), nullable=True)
    is_active = Column(Boolean, default=True)
    
    # 관계 - 기존 테이블과 연결
    flows = relationship("Flow", back_populates="owner", lazy="selectin")
    tokens = relationship("UserToken", back_populates="user", cascade="all, delete-orphan")
    activity_logs = relationship("ActivityLog", back_populates="user", lazy="dynamic")
```

### 1.3 UserToken 모델

```python
# models/user_token.py
from sqlalchemy import Column, String, DateTime, ForeignKey, JSON
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func
from .base import Base
import uuid

class UserToken(Base):
    __tablename__ = "user_tokens"
    
    id = Column(String(36), primary_key=True, default=lambda: str(uuid.uuid4()))
    user_id = Column(String(36), ForeignKey("users.id", ondelete="CASCADE"), nullable=False)
    
    token_type = Column(String(50))  # 'access', 'refresh', 'api_key'
    token_hash = Column(String(255), nullable=False, index=True)
    
    device_info = Column(JSON, nullable=True)  # { os, version, device_id }
    ip_address = Column(String(45), nullable=True)
    
    issued_at = Column(DateTime(timezone=True), server_default=func.now())
    expires_at = Column(DateTime(timezone=True), nullable=False)
    revoked_at = Column(DateTime(timezone=True), nullable=True)
    
    # 관계
    user = relationship("User", back_populates="tokens")
```

### 1.4 ActivityLog 모델

```python
# models/activity_log.py
from sqlalchemy import Column, String, DateTime, ForeignKey, JSON, BigInteger, Text
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func
from .base import Base

class ActivityLog(Base):
    __tablename__ = "activity_logs"
    
    id = Column(BigInteger, primary_key=True, autoincrement=True)
    user_id = Column(String(36), ForeignKey("users.id"), nullable=True, index=True)
    
    action = Column(String(100), nullable=False, index=True)  # 'login', 'logout', 'component_install', etc.
    resource_type = Column(String(100), nullable=True)  # 'component', 'flow', 'project'
    resource_id = Column(String(255), nullable=True)
    
    metadata = Column(JSON, nullable=True)
    
    ip_address = Column(String(45), nullable=True)
    user_agent = Column(Text, nullable=True)
    
    created_at = Column(DateTime(timezone=True), server_default=func.now(), index=True)
    
    # 관계
    user = relationship("User", back_populates="activity_logs")
```

### 1.5 TierConfig 모델

```python
# models/tier_config.py
from sqlalchemy import Column, String, Integer, Numeric, JSON
from .base import Base

class TierConfig(Base):
    __tablename__ = "tier_configs"
    
    tier = Column(String(50), primary_key=True)
    display_name = Column(String(100))
    
    # 제한 설정 (-1 = 무제한)
    max_projects = Column(Integer, default=3)
    max_components = Column(Integer, default=10)
    max_api_calls_day = Column(Integer, default=100)
    max_api_calls_month = Column(Integer, default=1000)
    max_storage_mb = Column(Integer, default=500)
    
    # 기능 플래그
    features = Column(JSON, default={})
    
    # 가격 (NULL = 문의)
    price_monthly = Column(Numeric(10, 2), nullable=True)
    price_yearly = Column(Numeric(10, 2), nullable=True)
```

### 1.6 기존 Flow 모델 수정

```python
# models/flow.py (기존 파일 수정)
from sqlalchemy import Column, String, ForeignKey
from sqlalchemy.orm import relationship

class Flow(Base):
    __tablename__ = "flows"
    
    # ... 기존 컬럼들 유지 ...
    
    # 추가: 소유자 연결
    owner_id = Column(String(36), ForeignKey("users.id"), nullable=True, index=True)
    owner = relationship("User", back_populates="flows")
```

---

## 2. FastAPI 인증 라우터

### 2.1 Google OAuth 설정

```python
# core/config.py
from pydantic_settings import BaseSettings

class Settings(BaseSettings):
    GOOGLE_CLIENT_ID: str
    GOOGLE_CLIENT_SECRET: str
    JWT_SECRET: str
    JWT_REFRESH_SECRET: str
    JWT_ALGORITHM: str = "HS256"
    ACCESS_TOKEN_EXPIRE_MINUTES: int = 60
    REFRESH_TOKEN_EXPIRE_DAYS: int = 30
    
    class Config:
        env_file = ".env"

settings = Settings()
```

### 2.2 인증 서비스

```python
# services/auth_service.py
from datetime import datetime, timedelta
from typing import Optional
import jwt
import hashlib
from google.oauth2 import id_token
from google.auth.transport import requests
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select

from models.user import User
from models.user_token import UserToken
from models.activity_log import ActivityLog
from core.config import settings

class AuthService:
    def __init__(self, db: AsyncSession):
        self.db = db
    
    async def verify_google_token(self, credential: str) -> dict:
        """Google ID 토큰 검증"""
        try:
            idinfo = id_token.verify_oauth2_token(
                credential, 
                requests.Request(), 
                settings.GOOGLE_CLIENT_ID
            )
            return idinfo
        except ValueError:
            raise ValueError("Invalid Google token")
    
    async def get_or_create_user(self, google_data: dict) -> tuple[User, bool]:
        """사용자 조회 또는 생성"""
        result = await self.db.execute(
            select(User).where(User.google_id == google_data["sub"])
        )
        user = result.scalar_one_or_none()
        
        is_new = False
        if not user:
            is_new = True
            user = User(
                google_id=google_data["sub"],
                email=google_data["email"],
                name=google_data.get("name"),
                profile_image=google_data.get("picture"),
            )
            self.db.add(user)
            await self.db.flush()
        
        return user, is_new
    
    def create_access_token(self, user: User) -> str:
        """Access Token 생성"""
        expires = datetime.utcnow() + timedelta(minutes=settings.ACCESS_TOKEN_EXPIRE_MINUTES)
        payload = {
            "sub": user.id,
            "email": user.email,
            "tier": user.tier.value,
            "exp": expires,
            "type": "access"
        }
        return jwt.encode(payload, settings.JWT_SECRET, algorithm=settings.JWT_ALGORITHM)
    
    def create_refresh_token(self, user: User) -> str:
        """Refresh Token 생성"""
        expires = datetime.utcnow() + timedelta(days=settings.REFRESH_TOKEN_EXPIRE_DAYS)
        payload = {
            "sub": user.id,
            "exp": expires,
            "type": "refresh"
        }
        return jwt.encode(payload, settings.JWT_REFRESH_SECRET, algorithm=settings.JWT_ALGORITHM)
    
    async def save_refresh_token(self, user_id: str, token: str, ip: str = None):
        """Refresh Token 저장"""
        token_hash = hashlib.sha256(token.encode()).hexdigest()
        expires = datetime.utcnow() + timedelta(days=settings.REFRESH_TOKEN_EXPIRE_DAYS)
        
        user_token = UserToken(
            user_id=user_id,
            token_type="refresh",
            token_hash=token_hash,
            ip_address=ip,
            expires_at=expires,
        )
        self.db.add(user_token)
    
    async def log_activity(self, user_id: str, action: str, metadata: dict = None, ip: str = None):
        """활동 로그 기록"""
        log = ActivityLog(
            user_id=user_id,
            action=action,
            metadata=metadata,
            ip_address=ip,
        )
        self.db.add(log)
```

### 2.3 인증 라우터

```python
# routers/auth.py
from fastapi import APIRouter, Depends, HTTPException, Request
from sqlalchemy.ext.asyncio import AsyncSession
from pydantic import BaseModel

from database import get_db
from services.auth_service import AuthService

router = APIRouter(prefix="/auth", tags=["Authentication"])

class GoogleAuthRequest(BaseModel):
    credential: str

class AuthResponse(BaseModel):
    access_token: str
    refresh_token: str
    user: dict

@router.post("/google", response_model=AuthResponse)
async def google_login(
    request: Request,
    body: GoogleAuthRequest,
    db: AsyncSession = Depends(get_db)
):
    auth_service = AuthService(db)
    
    # Google 토큰 검증
    try:
        google_data = await auth_service.verify_google_token(body.credential)
    except ValueError:
        raise HTTPException(status_code=401, detail="Invalid Google token")
    
    # 사용자 조회/생성
    user, is_new = await auth_service.get_or_create_user(google_data)
    
    # 토큰 생성
    access_token = auth_service.create_access_token(user)
    refresh_token = auth_service.create_refresh_token(user)
    
    # Refresh Token 저장
    await auth_service.save_refresh_token(user.id, refresh_token, request.client.host)
    
    # 로그 기록
    action = "signup" if is_new else "login"
    await auth_service.log_activity(user.id, action, {"method": "google"}, request.client.host)
    
    # 마지막 로그인 업데이트
    user.last_login_at = datetime.utcnow()
    
    await db.commit()
    
    return {
        "access_token": access_token,
        "refresh_token": refresh_token,
        "user": {
            "id": user.id,
            "email": user.email,
            "name": user.name,
            "profile_image": user.profile_image,
            "tier": user.tier.value,
        }
    }
```

### 2.4 인증 의존성 (미들웨어)

```python
# dependencies/auth.py
from fastapi import Depends, HTTPException, Header
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select
import jwt

from database import get_db
from models.user import User
from models.enums import TierType
from core.config import settings

async def get_current_user(
    authorization: str = Header(...),
    db: AsyncSession = Depends(get_db)
) -> User:
    """현재 로그인된 사용자 가져오기"""
    if not authorization.startswith("Bearer "):
        raise HTTPException(status_code=401, detail="Invalid authorization header")
    
    token = authorization[7:]
    
    try:
        payload = jwt.decode(token, settings.JWT_SECRET, algorithms=[settings.JWT_ALGORITHM])
    except jwt.ExpiredSignatureError:
        raise HTTPException(status_code=401, detail="Token expired")
    except jwt.InvalidTokenError:
        raise HTTPException(status_code=401, detail="Invalid token")
    
    result = await db.execute(select(User).where(User.id == payload["sub"]))
    user = result.scalar_one_or_none()
    
    if not user or not user.is_active:
        raise HTTPException(status_code=401, detail="User not found or inactive")
    
    return user

def require_tier(*allowed_tiers: TierType):
    """등급별 접근 제어 의존성"""
    async def tier_checker(user: User = Depends(get_current_user)) -> User:
        if user.tier not in allowed_tiers:
            raise HTTPException(
                status_code=403,
                detail=f"Upgrade required. Current: {user.tier.value}, Required: {[t.value for t in allowed_tiers]}"
            )
        return user
    return tier_checker
```

### 2.5 사용 예시

```python
# routers/components.py
from fastapi import APIRouter, Depends
from dependencies.auth import get_current_user, require_tier
from models.user import User
from models.enums import TierType

router = APIRouter(prefix="/components", tags=["Components"])

@router.get("/")
async def list_components(user: User = Depends(get_current_user)):
    """모든 로그인 사용자 접근 가능"""
    return {"user_tier": user.tier.value}

@router.post("/ai-generate")
async def ai_generate_component(user: User = Depends(require_tier(TierType.PRO, TierType.ENTERPRISE))):
    """Pro 이상만 접근 가능"""
    return {"message": "AI component generated"}
```

---

## 3. 로그 수집

### 3.1 로그 서비스

```python
# services/log_service.py
from typing import List
from sqlalchemy.ext.asyncio import AsyncSession
from models.activity_log import ActivityLog

class LogService:
    def __init__(self, db: AsyncSession):
        self.db = db
    
    async def bulk_insert_logs(self, user_id: str, logs: List[dict]):
        """Electron 앱에서 배치로 전송된 로그 저장"""
        for log_data in logs:
            log = ActivityLog(
                user_id=user_id,
                action=log_data["action"],
                resource_type=log_data.get("resource_type"),
                resource_id=log_data.get("resource_id"),
                metadata=log_data.get("metadata"),
            )
            self.db.add(log)
        await self.db.flush()
```

---

## 4. 기본 등급 데이터 초기화

```python
# scripts/init_tiers.py
async def init_tier_configs(db: AsyncSession):
    """기본 등급 설정 삽입"""
    tiers = [
        TierConfig(
            tier="free", display_name="Free",
            max_projects=3, max_components=10,
            max_api_calls_day=100, max_api_calls_month=1000,
            max_storage_mb=500,
            features={"ai_assistant": False},
            price_monthly=0, price_yearly=0
        ),
        TierConfig(
            tier="basic", display_name="Basic",
            max_projects=10, max_components=50,
            max_api_calls_day=1000, max_api_calls_month=10000,
            max_storage_mb=5000,
            features={"ai_assistant": True},
            price_monthly=9.99, price_yearly=99
        ),
        TierConfig(
            tier="pro", display_name="Pro",
            max_projects=-1, max_components=-1,
            max_api_calls_day=10000, max_api_calls_month=100000,
            max_storage_mb=50000,
            features={"ai_assistant": True, "priority_support": True},
            price_monthly=29.99, price_yearly=299
        ),
        TierConfig(
            tier="enterprise", display_name="Enterprise",
            max_projects=-1, max_components=-1,
            max_api_calls_day=-1, max_api_calls_month=-1,
            max_storage_mb=-1,
            features={"ai_assistant": True, "priority_support": True, "custom_integrations": True},
            price_monthly=None, price_yearly=None
        ),
    ]
    
    for tier in tiers:
        db.add(tier)
    await db.commit()
```

---

## 5. AI 크레딧 (Credits) 시스템

> **용어 정리**
> - **JWT Token**: 인증용 (로그인 상태 유지)
> - **Credits**: AI 사용 과금용 (LLM API 호출 시 차감)

### 5.1 UserCredits 모델

```python
# models/user_credits.py
from sqlalchemy import Column, String, Integer, Numeric, DateTime, ForeignKey
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func
from .base import Base
import uuid

class UserCredits(Base):
    __tablename__ = "user_credits"
    
    id = Column(String(36), primary_key=True, default=lambda: str(uuid.uuid4()))
    user_id = Column(String(36), ForeignKey("users.id", ondelete="CASCADE"), unique=True, nullable=False)
    
    # 크레딧 잔액
    balance = Column(Integer, default=0)  # 현재 보유 크레딧
    
    # 누적 통계
    total_purchased = Column(Integer, default=0)  # 총 구매량
    total_used = Column(Integer, default=0)       # 총 사용량
    total_bonus = Column(Integer, default=0)      # 총 보너스 지급량
    
    # 시간 정보
    created_at = Column(DateTime(timezone=True), server_default=func.now())
    updated_at = Column(DateTime(timezone=True), onupdate=func.now())
    
    # 관계
    user = relationship("User", back_populates="credits")
    transactions = relationship("CreditTransaction", back_populates="user_credits", lazy="dynamic")
```

### 5.2 CreditTransaction 모델 (크레딧 거래 내역)

```python
# models/credit_transaction.py
from sqlalchemy import Column, String, Integer, DateTime, ForeignKey, JSON, Enum
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func
from .base import Base
import enum
import uuid

class TransactionType(str, enum.Enum):
    PURCHASE = "purchase"       # 구매
    USE = "use"                 # 사용
    REFUND = "refund"           # 환불
    BONUS = "bonus"             # 보너스
    EXPIRE = "expire"           # 만료
    ADMIN_ADJUST = "admin"      # 관리자 조정

class CreditTransaction(Base):
    __tablename__ = "credit_transactions"
    
    id = Column(String(36), primary_key=True, default=lambda: str(uuid.uuid4()))
    user_credits_id = Column(String(36), ForeignKey("user_credits.id", ondelete="CASCADE"), nullable=False)
    
    # 거래 정보
    transaction_type = Column(Enum(TransactionType), nullable=False)
    amount = Column(Integer, nullable=False)  # 양수: 충전, 음수: 차감
    balance_after = Column(Integer, nullable=False)  # 거래 후 잔액
    
    # 상세 정보
    description = Column(String(255))
    metadata = Column(JSON, nullable=True)  # { model: "gpt-4", prompt_tokens: 100, completion_tokens: 50 }
    
    # 참조 (결제, AI 요청 등)
    reference_type = Column(String(50), nullable=True)  # 'payment', 'ai_request', 'subscription'
    reference_id = Column(String(255), nullable=True)
    
    created_at = Column(DateTime(timezone=True), server_default=func.now(), index=True)
    
    # 관계
    user_credits = relationship("UserCredits", back_populates="transactions")
```

### 5.3 User 모델에 관계 추가

```python
# models/user.py 수정
class User(Base):
    # ... 기존 필드들 ...
    
    # 크레딧 관계 추가
    credits = relationship("UserCredits", back_populates="user", uselist=False)
```

### 5.4 크레딧 서비스

```python
# services/credit_service.py
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select
from models.user_credits import UserCredits
from models.credit_transaction import CreditTransaction, TransactionType

class CreditService:
    def __init__(self, db: AsyncSession):
        self.db = db
    
    async def get_or_create_credits(self, user_id: str) -> UserCredits:
        """사용자 크레딧 조회 또는 생성"""
        result = await self.db.execute(
            select(UserCredits).where(UserCredits.user_id == user_id)
        )
        credits = result.scalar_one_or_none()
        
        if not credits:
            credits = UserCredits(user_id=user_id, balance=0)
            self.db.add(credits)
            await self.db.flush()
        
        return credits
    
    async def get_balance(self, user_id: str) -> int:
        """크레딧 잔액 조회"""
        credits = await self.get_or_create_credits(user_id)
        return credits.balance
    
    async def add_credits(
        self, 
        user_id: str, 
        amount: int, 
        transaction_type: TransactionType,
        description: str = None,
        reference_type: str = None,
        reference_id: str = None,
        metadata: dict = None
    ) -> CreditTransaction:
        """크레딧 추가 (구매, 보너스 등)"""
        credits = await self.get_or_create_credits(user_id)
        credits.balance += amount
        
        if transaction_type == TransactionType.PURCHASE:
            credits.total_purchased += amount
        elif transaction_type == TransactionType.BONUS:
            credits.total_bonus += amount
        
        transaction = CreditTransaction(
            user_credits_id=credits.id,
            transaction_type=transaction_type,
            amount=amount,
            balance_after=credits.balance,
            description=description,
            reference_type=reference_type,
            reference_id=reference_id,
            metadata=metadata,
        )
        self.db.add(transaction)
        return transaction
    
    async def use_credits(
        self, 
        user_id: str, 
        amount: int,
        description: str,
        metadata: dict = None  # { model, prompt_tokens, completion_tokens }
    ) -> CreditTransaction:
        """크레딧 사용 (AI 호출 등)"""
        credits = await self.get_or_create_credits(user_id)
        
        if credits.balance < amount:
            raise ValueError(f"Insufficient credits. Balance: {credits.balance}, Required: {amount}")
        
        credits.balance -= amount
        credits.total_used += amount
        
        transaction = CreditTransaction(
            user_credits_id=credits.id,
            transaction_type=TransactionType.USE,
            amount=-amount,
            balance_after=credits.balance,
            description=description,
            metadata=metadata,
        )
        self.db.add(transaction)
        return transaction
    
    async def check_and_reserve(self, user_id: str, estimated_credits: int) -> bool:
        """크레딧 사전 확인 (AI 호출 전)"""
        balance = await self.get_balance(user_id)
        return balance >= estimated_credits
```

### 5.5 크레딧 체크 의존성

```python
# dependencies/credits.py
from fastapi import Depends, HTTPException
from sqlalchemy.ext.asyncio import AsyncSession
from database import get_db
from models.user import User
from services.credit_service import CreditService
from dependencies.auth import get_current_user

async def require_credits(min_credits: int = 1):
    """최소 크레딧 보유 확인"""
    async def checker(
        user: User = Depends(get_current_user),
        db: AsyncSession = Depends(get_db)
    ) -> User:
        credit_service = CreditService(db)
        balance = await credit_service.get_balance(user.id)
        
        if balance < min_credits:
            raise HTTPException(
                status_code=402,  # Payment Required
                detail={
                    "error": "Insufficient credits",
                    "balance": balance,
                    "required": min_credits,
                }
            )
        return user
    return checker
```

### 5.6 AI 엔드포인트에서 크레딧 사용

```python
# routers/ai.py
from fastapi import APIRouter, Depends
from sqlalchemy.ext.asyncio import AsyncSession
from database import get_db
from models.user import User
from services.credit_service import CreditService
from dependencies.auth import get_current_user
from dependencies.credits import require_credits

router = APIRouter(prefix="/ai", tags=["AI"])

# 모델별 크레딧 비용
CREDIT_COSTS = {
    "gpt-4": 10,
    "gpt-4-turbo": 5,
    "gpt-3.5-turbo": 1,
    "claude-3-opus": 15,
    "claude-3-sonnet": 3,
}

@router.post("/generate")
async def generate_with_ai(
    model: str,
    prompt: str,
    user: User = Depends(require_credits(1)),
    db: AsyncSession = Depends(get_db)
):
    credit_service = CreditService(db)
    
    # 모델별 비용 계산
    cost = CREDIT_COSTS.get(model, 1)
    
    # 잔액 확인
    balance = await credit_service.get_balance(user.id)
    if balance < cost:
        raise HTTPException(status_code=402, detail="Insufficient credits")
    
    # AI 호출 (실제 구현)
    # result = await call_llm(model, prompt)
    result = {"response": "AI generated response..."}
    
    # 크레딧 차감
    await credit_service.use_credits(
        user_id=user.id,
        amount=cost,
        description=f"AI generation using {model}",
        metadata={
            "model": model,
            "prompt_length": len(prompt),
        }
    )
    
    await db.commit()
    
    return {
        "result": result,
        "credits_used": cost,
        "balance_remaining": balance - cost,
    }
```

### 5.7 크레딧 가격표 (예시)

| 패키지 | 크레딧 | 가격 (USD) | 단가 |
|--------|--------|------------|------|
| Starter | 100 | $5 | $0.05 |
| Basic | 500 | $20 | $0.04 |
| Pro | 2,000 | $60 | $0.03 |
| Enterprise | 10,000 | $200 | $0.02 |

### 5.8 모델별 크레딧 소비량 (예시)

| 모델 | 크레딧/요청 | 설명 |
|------|-------------|------|
| GPT-3.5 Turbo | 1 | 기본 모델 |
| GPT-4 Turbo | 5 | 고급 모델 |
| GPT-4 | 10 | 최고급 모델 |
| Claude 3 Sonnet | 3 | Anthropic 중급 |
| Claude 3 Opus | 15 | Anthropic 최고급 |

---

## 6. 필요 패키지

```bash
pip install google-auth google-auth-oauthlib pyjwt[crypto]
```

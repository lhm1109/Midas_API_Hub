import express from 'express';
import db from '../database.js';

const router = express.Router();

// 현재 작업 중인 데이터 조회 (임시 저장소)
// 이는 버전과 별도로 실시간 편집 데이터를 저장
const workingData = {
  manualData: null,
  specData: null,
  builderData: null,
  runnerData: null,
};

// Manual Data
router.get('/manual', (req, res) => {
  res.json(workingData.manualData);
});

router.post('/manual', (req, res) => {
  workingData.manualData = req.body;
  res.json({ message: 'Manual data saved' });
});

// Spec Data
router.get('/spec', (req, res) => {
  res.json(workingData.specData);
});

router.post('/spec', (req, res) => {
  workingData.specData = req.body;
  res.json({ message: 'Spec data saved' });
});

// Builder Data
router.get('/builder', (req, res) => {
  res.json(workingData.builderData);
});

router.post('/builder', (req, res) => {
  workingData.builderData = req.body;
  res.json({ message: 'Builder data saved' });
});

// Runner Data
router.get('/runner', (req, res) => {
  res.json(workingData.runnerData);
});

router.post('/runner', (req, res) => {
  workingData.runnerData = req.body;
  res.json({ message: 'Runner data saved' });
});

// 모든 작업 데이터 조회
router.get('/all', (req, res) => {
  res.json(workingData);
});

// 모든 작업 데이터 초기화
router.delete('/all', (req, res) => {
  workingData.manualData = null;
  workingData.specData = null;
  workingData.builderData = null;
  workingData.runnerData = null;
  res.json({ message: 'All working data cleared' });
});

export default router;










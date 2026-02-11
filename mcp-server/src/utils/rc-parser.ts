/**
 * Windows RC (Resource) File Parser
 * Extracts DIALOG/DIALOGEX definitions and their controls
 */

export interface RCControl {
    id: string;           // e.g., "IDC_EDIT_NODE_ID"
    type: string;         // EDITTEXT, COMBOBOX, CHECKBOX, LTEXT, etc.
    caption?: string;     // Text for static/button/checkbox/radio
    x: number;
    y: number;
    w: number;
    h: number;
    styles?: string;
    // Associated label text (resolved from nearby LTEXT)
    labelText?: string;
}

export interface RCDialog {
    id: string;           // e.g., "IDD_TM_NODE_CREATE"
    caption?: string;     // Dialog title
    x: number;
    y: number;
    width: number;
    height: number;
    font?: string;
    controls: RCControl[];
}

/**
 * Parse all DIALOG/DIALOGEX definitions from RC file content
 */
export function parseRCFile(content: string): RCDialog[] {
    const dialogs: RCDialog[] = [];

    // Normalize line endings
    const normalized = content.replace(/\r\n/g, '\n').replace(/\r/g, '\n');
    const lines = normalized.split('\n');

    let i = 0;
    while (i < lines.length) {
        const line = lines[i].trim();

        // Match: IDD_xxx DIALOGEX x, y, w, h
        //    or: IDD_xxx DIALOG  x, y, w, h
        const dialogMatch = line.match(/^(IDD_\w+)\s+DIALOGEX?\s+([\d\s,]+)/);
        if (dialogMatch) {
            const dialogId = dialogMatch[1];
            const coords = parseNumbers(dialogMatch[2]);
            const dialog: RCDialog = {
                id: dialogId,
                x: coords[0] ?? 0,
                y: coords[1] ?? 0,
                width: coords[2] ?? 0,
                height: coords[3] ?? 0,
                controls: [],
            };

            // Read dialog properties (CAPTION, FONT) until BEGIN
            i++;
            while (i < lines.length) {
                const propLine = lines[i].trim();
                if (propLine.startsWith('CAPTION')) {
                    const captionMatch = propLine.match(/CAPTION\s+"([^"]*)"/);
                    if (captionMatch) dialog.caption = captionMatch[1];
                } else if (propLine.startsWith('FONT')) {
                    const fontMatch = propLine.match(/FONT\s+(.*)/);
                    if (fontMatch) dialog.font = fontMatch[1].trim();
                } else if (propLine === 'BEGIN') {
                    break;
                }
                i++;
            }

            // Read controls until END
            i++;
            const controls = parseControls(lines, i);
            dialog.controls = controls.controls;
            i = controls.nextLine;

            // Associate LTEXT labels with nearby controls
            associateLabels(dialog.controls);

            dialogs.push(dialog);
        } else {
            i++;
        }
    }

    return dialogs;
}

/**
 * Find a specific dialog by ID in RC file content
 */
export function findDialogInRC(content: string, dialogId: string): RCDialog | null {
    const dialogs = parseRCFile(content);
    return dialogs.find(d => d.id === dialogId) ?? null;
}

/**
 * Parse control statements inside a dialog BEGIN...END block
 */
function parseControls(lines: string[], startLine: number): { controls: RCControl[]; nextLine: number } {
    const controls: RCControl[] = [];
    let i = startLine;

    while (i < lines.length) {
        const raw = lines[i];
        const line = raw.trim();

        if (line === 'END') {
            i++;
            break;
        }

        // Skip empty lines and comments
        if (!line || line.startsWith('//') || line.startsWith('/*')) {
            i++;
            continue;
        }

        // Handle line continuation (backslash at end)
        let fullLine = line;
        while (fullLine.endsWith('\\') && i + 1 < lines.length) {
            fullLine = fullLine.slice(0, -1) + ' ' + lines[++i].trim();
        }

        const control = parseControlLine(fullLine);
        if (control) {
            controls.push(control);
        }

        i++;
    }

    return { controls, nextLine: i };
}

/**
 * Parse a single control line
 *
 * Formats:
 *   EDITTEXT        IDC_EDIT, x, y, w, h [,styles]
 *   LTEXT           "Caption", IDC_STATIC, x, y, w, h [,styles]
 *   CONTROL         "Caption", IDC_XXX, "Button", styles, x, y, w, h
 *   COMBOBOX        IDC_COMBO, x, y, w, h [,styles]
 *   GROUPBOX        "Caption", IDC_STATIC, x, y, w, h [,styles]
 *   PUSHBUTTON/DEFPUSHBUTTON "Caption", IDC_BTN, x, y, w, h
 *   ICON            IDC_ICON, IDC_XXX, x, y, w, h
 */
function parseControlLine(line: string): RCControl | null {
    // CONTROL "caption", ID, "class", styles, x, y, w, h
    const controlMatch = line.match(/^CONTROL\s+"([^"]*)",\s*(\w+),\s*"([^"]*)",\s*([^,]+),\s*([\d\s,\-]+)/);
    if (controlMatch) {
        const caption = controlMatch[1];
        const id = controlMatch[2];
        const className = controlMatch[3];
        const styles = controlMatch[4].trim();
        const coords = parseNumbers(controlMatch[5]);
        const type = resolveButtonType(className, styles, caption);

        return {
            id,
            type,
            caption: caption || undefined,
            x: coords[0] ?? 0,
            y: coords[1] ?? 0,
            w: coords[2] ?? 0,
            h: coords[3] ?? 0,
            styles,
        };
    }

    // LTEXT/RTEXT/CTEXT "caption", ID, x, y, w, h
    const staticMatch = line.match(/^(LTEXT|RTEXT|CTEXT|ICON)\s+"([^"]*)",\s*(\w+),\s*([\d\s,\-]+)/);
    if (staticMatch) {
        const type = staticMatch[1];
        const caption = staticMatch[2];
        const id = staticMatch[3];
        const coords = parseNumbers(staticMatch[4]);

        return {
            id,
            type,
            caption: caption || undefined,
            x: coords[0] ?? 0,
            y: coords[1] ?? 0,
            w: coords[2] ?? 0,
            h: coords[3] ?? 0,
        };
    }

    // EDITTEXT/COMBOBOX/LISTBOX IDC_XXX, x, y, w, h [,styles]
    const inputMatch = line.match(/^(EDITTEXT|COMBOBOX|LISTBOX|SCROLLBAR)\s+(\w+),\s*([\d\s,\-]+)/);
    if (inputMatch) {
        const type = inputMatch[1];
        const id = inputMatch[2];
        const rest = inputMatch[3];
        // rest may have styles after 4th number, e.g. "5,10,100,14,ES_AUTOHSCROLL"
        const coordsAndStyles = rest.split(',');
        const coords = coordsAndStyles.slice(0, 4).map(s => parseInt(s.trim(), 10));
        const styles = coordsAndStyles.slice(4).join(',').trim();

        return {
            id,
            type,
            x: coords[0] ?? 0,
            y: coords[1] ?? 0,
            w: coords[2] ?? 0,
            h: coords[3] ?? 0,
            styles: styles || undefined,
        };
    }

    // GROUPBOX "caption", IDC_STATIC, x, y, w, h
    const groupMatch = line.match(/^GROUPBOX\s+"([^"]*)",\s*(\w+),\s*([\d\s,\-]+)/);
    if (groupMatch) {
        const caption = groupMatch[1];
        const id = groupMatch[2];
        const coords = parseNumbers(groupMatch[3]);

        return {
            id,
            type: 'GROUPBOX',
            caption: caption || undefined,
            x: coords[0] ?? 0,
            y: coords[1] ?? 0,
            w: coords[2] ?? 0,
            h: coords[3] ?? 0,
        };
    }

    // PUSHBUTTON/DEFPUSHBUTTON "caption", IDC_BTN, x, y, w, h
    const buttonMatch = line.match(/^(DEFPUSHBUTTON|PUSHBUTTON|AUTORADIOBUTTON|AUTOCHECKBOX|AUTO3STATE|CHECKBOX)\s+"([^"]*)",\s*(\w+),\s*([\d\s,\-]+)/);
    if (buttonMatch) {
        const type = buttonMatch[1];
        const caption = buttonMatch[2];
        const id = buttonMatch[3];
        const coords = parseNumbers(buttonMatch[4]);

        return {
            id,
            type,
            caption: caption || undefined,
            x: coords[0] ?? 0,
            y: coords[1] ?? 0,
            w: coords[2] ?? 0,
            h: coords[3] ?? 0,
        };
    }

    return null;
}

/**
 * Resolve the actual control type from CONTROL's class and styles
 */
function resolveButtonType(className: string, styles: string, caption: string): string {
    if (className === 'Button') {
        if (styles.includes('BS_AUTOCHECKBOX') || styles.includes('BS_CHECKBOX')) {
            return 'CHECKBOX';
        }
        if (styles.includes('BS_AUTORADIOBUTTON') || styles.includes('BS_RADIOBUTTON')) {
            return 'RADIOBUTTON';
        }
        if (styles.includes('BS_AUTO3STATE') || styles.includes('BS_3STATE')) {
            return 'CHECKBOX3STATE';
        }
        if (styles.includes('BS_GROUPBOX')) {
            return 'GROUPBOX';
        }
        return 'BUTTON';
    }
    if (className === 'Static') return 'LTEXT';
    if (className === 'Edit') return 'EDITTEXT';
    if (className === 'ComboBox') return 'COMBOBOX';
    if (className === 'ListBox') return 'LISTBOX';
    if (className === 'msctls_updown32') return 'SPINCONTROL';
    if (className === 'msctls_progress32') return 'PROGRESSBAR';
    if (className === 'SysSlider32' || className === 'msctls_trackbar32') return 'SLIDER';
    if (className === 'SysTabControl32') return 'TABCONTROL';
    return className.toUpperCase();
}

/**
 * Associate LTEXT/RTEXT labels with nearby input controls
 * Uses vertical proximity: if a static is directly above or to the left of a control
 */
function associateLabels(controls: RCControl[]): void {
    const staticControls = controls.filter(c =>
        ['LTEXT', 'RTEXT', 'CTEXT'].includes(c.type) && c.caption
    );
    const inputControls = controls.filter(c =>
        !['LTEXT', 'RTEXT', 'CTEXT', 'GROUPBOX', 'PUSHBUTTON', 'DEFPUSHBUTTON', 'IDC_STATIC'].includes(c.type) &&
        c.id !== 'IDC_STATIC'
    );

    for (const input of inputControls) {
        // Find nearest static label
        let bestLabel: RCControl | null = null;
        let bestScore = Infinity;

        for (const label of staticControls) {
            // Label should be above or to the left of the control
            const isAbove = label.y >= input.y - 14 && label.y <= input.y + 2;
            const isLeft = label.x < input.x && Math.abs(label.y - input.y) <= 8;

            if (!isAbove && !isLeft) continue;

            // Score: horizontal + vertical distance
            const dx = Math.abs(label.x + label.w - input.x);
            const dy = Math.abs(label.y - input.y);
            const score = dx + dy * 2;

            if (score < bestScore) {
                bestScore = score;
                bestLabel = label;
            }
        }

        if (bestLabel) {
            input.labelText = bestLabel.caption;
        }
    }
}

/**
 * Parse comma-separated numbers from a string
 */
function parseNumbers(str: string): number[] {
    return str.split(',').map(s => parseInt(s.trim(), 10)).filter(n => !isNaN(n));
}

/**
 * Get only meaningful controls (exclude IDC_STATIC labels)
 */
export function getMeaningfulControls(dialog: RCDialog): RCControl[] {
    return dialog.controls.filter(c => c.id !== 'IDC_STATIC');
}

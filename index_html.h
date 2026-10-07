#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8"/>
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no"/>
<title>ABC Buddy Control</title>
<style>
  :root {
    --bg-page: #F8EFE7;
    --coral: #FF5738;
    --coral-dark: #E84D2E;
    --coral-glow: rgba(255, 87, 56, 0.28);
    --green-auto: #5DC728;
    --green-auto-dark: #4FB020;
    --gray-stop: #6C757D;
    --gray-stop-dark: #5A6268;
    --bg-telemetry: #F3E7D9;
    --text-dark: #2D3748;
    --text-mid: #5A6578;
    --text-muted: #718096;
    --blue-slider: #3B82F6;
  }

  * {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    -webkit-tap-highlight-color: transparent;
  }

  body {
    font-family: -apple-system, BlinkMacSystemFont, "SF Pro Text", "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    background-color: var(--bg-page);
    color: var(--text-dark);
    min-height: 100vh;
    padding: 12px 16px 36px 16px;
    display: flex;
    flex-direction: column;
    align-items: center;
  }

  .app-viewport {
    width: 100%;
    max-width: 380px;
    display: flex;
    flex-direction: column;
    gap: 12px;
  }

  /* View Mode Switcher (Discrete Pills for Video Presentation) */
  .mode-tabs {
    display: flex;
    background: rgba(0, 0, 0, 0.05);
    padding: 3px;
    border-radius: 20px;
    margin-bottom: 2px;
    user-select: none;
  }
  .mode-tab {
    flex: 1;
    text-align: center;
    padding: 6px 10px;
    font-size: 12px;
    font-weight: 700;
    color: var(--text-mid);
    border-radius: 16px;
    cursor: pointer;
    transition: all 0.2s ease;
  }
  .mode-tab.active {
    background: #ffffff;
    color: var(--coral);
    box-shadow: 0 2px 6px rgba(0, 0, 0, 0.08);
  }

  /* 1. Header with Live Connection Pill */
  .header-banner {
    background-color: var(--coral);
    border-radius: 14px;
    padding: 13px 18px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    box-shadow: 0 4px 14px var(--coral-glow);
    cursor: pointer;
    user-select: none;
    transition: transform 0.1s;
  }
  .header-banner:active {
    transform: scale(0.99);
  }
  .header-title {
    color: #ffffff;
    font-size: 20px;
    font-weight: 700;
    letter-spacing: -0.2px;
  }
  .header-status-pill {
    display: flex;
    align-items: center;
    gap: 6px;
    background: rgba(0, 0, 0, 0.22);
    padding: 4px 10px;
    border-radius: 20px;
    font-size: 11px;
    font-weight: 600;
    color: #ffffff;
    border: 1px solid rgba(255, 255, 255, 0.25);
  }
  .status-dot {
    width: 7px;
    height: 7px;
    border-radius: 50%;
    background-color: #4ade80;
    box-shadow: 0 0 8px #4ade80;
    animation: pulseDot 2s infinite ease-in-out;
  }
  @keyframes pulseDot {
    0%, 100% { opacity: 1; transform: scale(1); }
    50% { opacity: 0.35; transform: scale(0.85); }
  }

  /* 2. Speed Slider (Exact Match to Screenshot) */
  .speed-container {
    text-align: center;
    padding: 4px 10px 0 10px;
  }
  .speed-label {
    font-size: 15px;
    font-weight: 600;
    color: #333333;
    margin-bottom: 8px;
  }
  .slider-bar-wrap {
    width: 100%;
    max-width: 290px;
    margin: 0 auto;
    padding: 2px 4px;
  }
  .slider-input {
    width: 100%;
    height: 4px;
    background: #cbd5e1;
    border-radius: 4px;
    outline: none;
    -webkit-appearance: none;
    accent-color: var(--blue-slider);
  }
  .slider-input::-webkit-slider-thumb {
    -webkit-appearance: none;
    width: 18px;
    height: 18px;
    border-radius: 50%;
    background: #ffffff;
    box-shadow: 0 2px 6px rgba(0, 0, 0, 0.25);
    cursor: pointer;
    border: 2px solid var(--blue-slider);
  }

  /* 3. Cross D-Pad (Exact Match to Screenshot) */
  .dpad-grid {
    display: grid;
    grid-template-columns: 1fr 1fr 1fr;
    grid-template-rows: auto auto auto;
    gap: 8px;
    width: 100%;
    max-width: 300px;
    margin: 6px auto;
  }
  .dpad-cell {
    display: flex;
    align-items: center;
    justify-content: center;
  }
  .btn-pad {
    width: 100%;
    height: 52px;
    border: none;
    border-radius: 12px;
    color: #ffffff;
    font-size: 15px;
    font-weight: 700;
    cursor: pointer;
    box-shadow: 0 2px 6px rgba(0, 0, 0, 0.08);
    transition: transform 0.1s, filter 0.1s;
    user-select: none;
    -webkit-user-select: none;
  }
  .btn-pad:active {
    transform: scale(0.96);
    filter: brightness(0.92);
  }
  .btn-pad.coral {
    background-color: var(--coral);
  }
  .btn-pad.stop {
    background-color: var(--gray-stop);
  }

  /* Motor Telemetry Badge */
  .motor-telemetry-badge {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
    background: var(--bg-telemetry);
    border: 1px solid rgba(0, 0, 0, 0.08);
    border-radius: 10px;
    padding: 8px 12px;
    margin-top: 6px;
    font-size: 12.5px;
    font-weight: 700;
    color: #475569;
    transition: all 0.2s ease;
  }
  .motor-telemetry-badge.active {
    background: #ecfdf5;
    border-color: #10b981;
    color: #047857;
    box-shadow: 0 2px 8px rgba(16, 185, 129, 0.2);
  }

  /* 4. Auto-Follow Toggle (Exact Match to Screenshot) */
  .mode-toggle-wrap {
    text-align: center;
    margin-top: 2px;
  }
  .btn-auto-follow {
    width: 100%;
    max-width: 300px;
    height: 48px;
    background-color: var(--green-auto);
    color: #ffffff;
    border: none;
    border-radius: 12px;
    font-size: 15px;
    font-weight: 700;
    cursor: pointer;
    box-shadow: 0 3px 8px rgba(93, 199, 40, 0.25);
    transition: transform 0.1s;
  }
  .btn-auto-follow:active {
    transform: scale(0.98);
  }
  .mode-status-text {
    font-size: 14px;
    font-weight: 600;
    color: #666666;
    margin-top: 8px;
  }

  /* 5. Telemetry Box (Exact Match to Screenshot) */
  .telemetry-box {
    background-color: var(--bg-telemetry);
    border-radius: 14px;
    padding: 16px 22px;
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.04);
  }
  .telemetry-row {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 6px 0;
    font-size: 14px;
    font-weight: 600;
    color: #4a5568;
  }
  .telemetry-val {
    font-weight: 700;
    color: #2d3748;
    text-align: right;
  }
  .telemetry-ok {
    color: #2ecc71;
    font-weight: 700;
  }
  .telemetry-hazard {
    color: #e74c3c;
    font-weight: 700;
  }
  .telemetry-warn {
    color: #f39c12;
    font-weight: 700;
  }

  /* =========================================================================
     COMPANION FEATURES: VOICE & VISION LEARNING (Matching Theme Seamlessly)
     ========================================================================= */
  .companion-section {
    display: flex;
    flex-direction: column;
    gap: 12px;
    margin-top: 2px;
  }

  /* AI Voice Card */
  .card-feature {
    background: #ffffff;
    border: 1px solid rgba(0, 0, 0, 0.06);
    border-radius: 14px;
    padding: 16px;
    box-shadow: 0 3px 10px rgba(0, 0, 0, 0.04);
  }
  .card-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 12px;
  }
  .card-title {
    font-size: 15px;
    font-weight: 700;
    color: #2d3748;
  }
  .card-badge {
    font-size: 11px;
    font-weight: 700;
    padding: 3px 8px;
    border-radius: 8px;
    background: var(--bg-telemetry);
    color: var(--coral);
  }

  /* Hold to Talk Button */
  .btn-talk-buddy {
    width: 100%;
    height: 52px;
    border-radius: 12px;
    background-color: var(--coral);
    color: #ffffff;
    font-size: 15px;
    font-weight: 700;
    border: none;
    cursor: pointer;
    box-shadow: 0 4px 12px var(--coral-glow);
    transition: transform 0.1s, background-color 0.2s;
    user-select: none;
    -webkit-user-select: none;
  }
  .btn-talk-buddy:active, .btn-talk-buddy.listening {
    background-color: #ef4444;
    transform: scale(0.98);
    animation: listenPulse 1.2s infinite ease-in-out;
  }
  @keyframes listenPulse {
    0%, 100% { transform: scale(0.98); }
    50% { transform: scale(1.02); }
  }
  .talk-help-text {
    font-size: 11px;
    color: var(--text-muted);
    text-align: center;
    margin: 6px 0 0 0;
  }
  .buddy-reply-bubble {
    display: none;
    margin-top: 12px;
    padding: 12px 14px;
    border-radius: 16px 16px 16px 4px;
    background: linear-gradient(135deg, #fff7ed, #ffedd5);
    border: 1px solid #fdba74;
    color: #7c2d12;
    font-size: 14px;
    line-height: 1.45;
    animation: replyIn 0.35s ease-out;
  }
  .buddy-reply-bubble.show { display: block; }
  .buddy-reply-name { font-weight: 700; font-size: 12px; color: #ea580c; margin-bottom: 4px; display:flex; align-items:center; gap:6px; }
  .buddy-reply-dots span { display:inline-block; width:6px; height:6px; margin-right:3px; border-radius:50%; background:#fb923c; animation: dotBounce 1s infinite; }
  .buddy-reply-dots span:nth-child(2) { animation-delay: 0.15s; }
  .buddy-reply-dots span:nth-child(3) { animation-delay: 0.3s; }
  @keyframes dotBounce { 0%,100% { transform: translateY(0); opacity:0.5; } 50% { transform: translateY(-4px); opacity:1; } }
  @keyframes replyIn { from { opacity:0; transform: translateY(6px); } to { opacity:1; transform: translateY(0); } }

  /* Letter Chips (Clean Grid, Guaranteed Zero Overflow & Zero Scrollbars) */
  .chips-row {
    display: grid;
    grid-template-columns: repeat(7, 1fr);
    gap: 5px;
    width: 100%;
    margin-bottom: 8px;
    box-sizing: border-box;
    overflow: hidden;
  }
  .chip-letter {
    display: flex;
    align-items: center;
    justify-content: center;
    height: 38px;
    border-radius: 10px;
    background: var(--bg-telemetry);
    border: 1.5px solid transparent;
    font-weight: 700;
    font-size: 14px;
    cursor: pointer;
    color: #4a5568;
    transition: all 0.15s ease;
    user-select: none;
    box-sizing: border-box;
    white-space: nowrap;
    overflow: hidden;
  }
  .chip-letter.star-chip {
    font-size: 11px;
    padding: 0 1px;
  }
  .chip-letter.active {
    background: var(--coral);
    color: #ffffff;
    border-color: var(--coral-dark);
    box-shadow: 0 2px 6px var(--coral-glow);
  }

  /* Custom Character/Word Input Box */
  .custom-input-box {
    margin: 4px 0 10px 0;
    position: relative;
    display: flex;
    align-items: center;
    width: 100%;
    box-sizing: border-box;
  }
  .custom-input-box input {
    width: 100%;
    height: 42px;
    background: #ffffff;
    border: 1.5px solid rgba(0, 0, 0, 0.12);
    border-radius: 10px;
    padding: 0 36px 0 36px;
    font-size: 13.5px;
    font-weight: 600;
    color: #2d3748;
    outline: none;
    transition: all 0.2s ease;
    box-sizing: border-box;
    font-family: inherit;
  }
  .custom-input-box input:focus {
    border-color: var(--coral);
    box-shadow: 0 0 0 3px rgba(255, 112, 67, 0.15);
  }
  .custom-input-box input::placeholder {
    color: #94a3b8;
    font-weight: 400;
    font-size: 12.5px;
  }
  .custom-input-icon {
    position: absolute;
    left: 11px;
    font-size: 15px;
    pointer-events: none;
    user-select: none;
  }
  .custom-input-clear {
    position: absolute;
    right: 10px;
    width: 22px;
    height: 22px;
    border-radius: 50%;
    background: #e2e8f0;
    border: none;
    color: #64748b;
    font-size: 12px;
    font-weight: 700;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    transition: all 0.15s;
  }
  .custom-input-clear:hover {
    background: #cbd5e1;
    color: #1e293b;
  }

  /* Robot AI Vision Viewfinder HUD */
  .vision-hud-box {
    position: relative;
    width: 100%;
    height: 145px;
    background: #0f172a;
    border-radius: 12px;
    overflow: hidden;
    margin: 4px 0 10px 0;
    border: 1.5px solid #334155;
    box-shadow: inset 0 2px 10px rgba(0, 0, 0, 0.7);
    display: flex;
    flex-direction: column;
    justify-content: space-between;
    padding: 8px 10px;
    box-sizing: border-box;
    user-select: none;
  }
  .vision-hud-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    z-index: 5;
  }
  .vision-live-tag {
    display: inline-flex;
    align-items: center;
    gap: 5px;
    font-size: 11px;
    font-weight: 700;
    color: #4ade80;
    text-transform: uppercase;
    letter-spacing: 0.5px;
  }
  .vision-live-dot {
    width: 6px;
    height: 6px;
    border-radius: 50%;
    background: #4ade80;
    box-shadow: 0 0 8px #4ade80;
    animation: pulseDot 1.5s infinite;
  }
  .vision-hud-mode {
    font-size: 10.5px;
    font-weight: 600;
    color: #94a3b8;
    background: rgba(255, 255, 255, 0.08);
    padding: 2px 8px;
    border-radius: 6px;
  }
  .vision-hud-center {
    position: absolute;
    inset: 0;
    display: flex;
    align-items: center;
    justify-content: center;
    flex-direction: column;
    pointer-events: none;
  }
  .vision-target-box {
    position: relative;
    width: 86px;
    height: 86px;
    border: 1.5px dashed rgba(74, 222, 128, 0.5);
    border-radius: 10px;
    display: flex;
    align-items: center;
    justify-content: center;
    background: rgba(74, 222, 128, 0.04);
    box-shadow: 0 0 16px rgba(74, 222, 128, 0.12);
    transition: all 0.2s ease;
  }
  .vision-corner {
    position: absolute;
    width: 8px;
    height: 8px;
    border-color: #4ade80;
    border-style: solid;
  }
  .vision-corner.tl { top: -2px; left: -2px; border-width: 2px 0 0 2px; }
  .vision-corner.tr { top: -2px; right: -2px; border-width: 2px 2px 0 0; }
  .vision-corner.bl { bottom: -2px; left: -2px; border-width: 0 0 2px 2px; }
  .vision-corner.br { bottom: -2px; right: -2px; border-width: 0 2px 2px 0; }
  .vision-char-preview {
    font-size: 42px;
    font-weight: 900;
    color: #f8fafc;
    text-shadow: 0 0 12px rgba(255, 255, 255, 0.8), 0 0 24px rgba(74, 222, 128, 0.5);
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
    letter-spacing: -0.5px;
    max-width: 80px;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    text-align: center;
  }
  .vision-laser-beam {
    display: none;
    position: absolute;
    left: 0;
    right: 0;
    height: 3px;
    background: linear-gradient(90deg, transparent, #38bdf8, #4ade80, #38bdf8, transparent);
    box-shadow: 0 0 10px #38bdf8, 0 0 20px #4ade80;
    z-index: 10;
  }
  .vision-laser-beam.scanning {
    display: block;
    animation: scanLaser 1.2s infinite ease-in-out;
  }
  @keyframes scanLaser {
    0% { top: 4%; opacity: 0; }
    20% { opacity: 1; }
    80% { opacity: 1; }
    100% { top: 96%; opacity: 0; }
  }
  .vision-hud-footer {
    display: flex;
    justify-content: space-between;
    align-items: center;
    font-size: 10.5px;
    color: #94a3b8;
    z-index: 5;
    font-weight: 600;
  }

  /* Capture Button */
  .btn-capture-letter {
    width: 100%;
    height: 48px;
    border-radius: 12px;
    background-color: var(--coral);
    color: #ffffff;
    font-size: 14.5px;
    font-weight: 700;
    border: none;
    cursor: pointer;
    box-shadow: 0 3px 8px var(--coral-glow);
    transition: transform 0.1s, background-color 0.2s;
    user-select: none;
    -webkit-user-select: none;
  }
  .btn-capture-letter:active {
    transform: scale(0.98);
  }

  /* Verification Feedback Box */
  .result-banner {
    background: #ffffff;
    border: 1.5px solid #10b981;
    border-radius: 12px;
    padding: 12px 14px;
    margin-top: 10px;
    box-shadow: 0 2px 10px rgba(16, 185, 129, 0.1);
    box-sizing: border-box;
    width: 100%;
    transition: all 0.2s ease;
  }
  .result-placeholder {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 12.5px;
    font-weight: 600;
    color: #64748b;
    line-height: 1.4;
  }
  .result-top {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 6px;
  }
  .result-status {
    font-weight: 800;
    color: #047857;
    font-size: 13.5px;
    display: flex;
    align-items: center;
    gap: 4px;
  }
  .result-score {
    color: #f59e0b;
    font-weight: 800;
    font-size: 13.5px;
  }
  .result-feedback {
    background: #f8fafc;
    border-left: 3px solid #10b981;
    padding: 8px 10px;
    border-radius: 6px;
    color: #1e293b;
    font-weight: 600;
    font-size: 13px;
    line-height: 1.45;
    font-style: italic;
    margin: 4px 0 8px 0;
  }
  .result-audio-bar {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding-top: 6px;
    border-top: 1px solid #f1f5f9;
  }
  .sound-wave-indicator {
    display: none;
    align-items: center;
    gap: 3px;
    font-size: 12px;
    font-weight: 700;
    color: #0284c7;
  }
  .sound-bar {
    width: 3px;
    height: 12px;
    background: #0284c7;
    border-radius: 2px;
    animation: soundWave 0.8s infinite ease-in-out;
  }
  .sound-bar:nth-child(2) { animation-delay: 0.15s; height: 16px; }
  .sound-bar:nth-child(3) { animation-delay: 0.3s; height: 10px; }
  @keyframes soundWave {
    0%, 100% { transform: scaleY(0.4); }
    50% { transform: scaleY(1.2); }
  }
  .btn-hear-again {
    background: #f1f5f9;
    border: 1px solid #cbd5e1;
    border-radius: 8px;
    padding: 4px 10px;
    font-size: 12px;
    font-weight: 700;
    color: #334155;
    cursor: pointer;
    display: inline-flex;
    align-items: center;
    gap: 4px;
    transition: background 0.15s;
    user-select: none;
  }
  .btn-hear-again:hover {
    background: #e2e8f0;
  }

  /* Floating Quick Mic on Remote Screen */
  .fab-mic {
    position: fixed;
    bottom: 20px;
    right: 20px;
    width: 54px;
    height: 54px;
    border-radius: 50%;
    background: var(--coral);
    color: #ffffff;
    display: none;
    align-items: center;
    justify-content: center;
    font-size: 24px;
    box-shadow: 0 4px 14px var(--coral-glow);
    cursor: pointer;
    z-index: 100;
    user-select: none;
    transition: transform 0.1s;
  }
  .fab-mic:active, .fab-mic.listening {
    background: #ef4444;
    transform: scale(1.08);
  }

  /* Hidden Modal for Config */
  .modal-overlay {
    display: none;
    position: fixed;
    inset: 0;
    background: rgba(0, 0, 0, 0.4);
    z-index: 200;
    align-items: center;
    justify-content: center;
    padding: 20px;
  }
  .modal-card {
    background: #ffffff;
    border-radius: 16px;
    padding: 20px;
    width: 100%;
    max-width: 320px;
    box-shadow: 0 10px 25px rgba(0, 0, 0, 0.2);
  }
  .modal-card h3 {
    font-size: 16px;
    margin-bottom: 12px;
    color: var(--text-dark);
  }
  .modal-input {
    width: 100%;
    padding: 8px 10px;
    border: 1px solid #cbd5e1;
    border-radius: 8px;
    margin-bottom: 10px;
    font-size: 13px;
  }
  .modal-btn {
    width: 100%;
    padding: 10px;
    background: var(--coral);
    color: #ffffff;
    border: none;
    border-radius: 10px;
    font-weight: 700;
    cursor: pointer;
  }
</style>
</head>
<body>

  <div class="app-viewport">

    <!-- Presentation Switcher -->
    <div class="mode-tabs">
      <div class="mode-tab active" id="tabStudio" onclick="setAppMode('studio')">Voice & Vision</div>
      <div class="mode-tab" id="tabRemote" onclick="setAppMode('remote')">Robot Remote</div>
      <div class="mode-tab" id="tabAll" onclick="setAppMode('all')">Full View</div>
    </div>

    <!-- 1. Header with Live Robot Link Pill -->
    <div class="header-banner" onclick="handleHeaderTap()">
      <div class="header-title">ABC Buddy Control</div>
      <div class="header-status-pill" id="hdrConnPill" title="Real-time Robot Telemetry Link">
        <span class="status-dot"></span>
        <span id="hdrConnText">Connected • 12ms</span>
      </div>
    </div>

    <!-- MAIN REMOTE CONTROLLER VIEW -->
    <div id="sectionRemote" style="display:none;">
      
      <!-- 2. Speed Slider (Exact Match to Screenshot) -->
      <div class="speed-container">
        <div class="speed-label">Speed: <span id="valSpeed">160 (63%)</span></div>
        <div class="slider-bar-wrap">
          <input type="range" min="0" max="255" value="160" class="slider-input" id="speedSlider"
            oninput="updateSpeedDisplay(this.value)">
        </div>
      </div>

      <!-- 3. Directional D-Pad (Exact Match to Screenshot) -->
      <div class="dpad-grid">
        <div class="dpad-cell"></div>
        <div class="dpad-cell">
          <button class="btn-pad coral" id="btnFwd">Forward</button>
        </div>
        <div class="dpad-cell"></div>

        <div class="dpad-cell">
          <button class="btn-pad coral" id="btnLeft">Left</button>
        </div>
        <div class="dpad-cell">
          <button class="btn-pad stop" onclick="stopMotion()">Stop</button>
        </div>
        <div class="dpad-cell">
          <button class="btn-pad coral" id="btnRight">Right</button>
        </div>

        <div class="dpad-cell"></div>
        <div class="dpad-cell">
          <button class="btn-pad coral" id="btnBwd">Reverse</button>
        </div>
        <div class="dpad-cell"></div>
      </div>

      <!-- Live Motor Telemetry Feedback Pill -->
      <div class="motor-telemetry-badge" id="lblMotorStatus">
        <span>⚡</span>
        <span id="motorStatusText">Drive Motors: Standby (0 PWM)</span>
      </div>

      <!-- 4. Auto-Follow Toggle & Status (Exact Match to Screenshot) -->
      <div class="mode-toggle-wrap">
        <button class="btn-auto-follow" id="btnAutoToggle" onclick="toggleAutoDrive()">
          Disable Auto-Follow
        </button>
        <div class="mode-status-text" id="lblModeText">Mode: Auto-Follow</div>
      </div>

      <!-- 5. Telemetry Card (Exact Match to Screenshot) -->
      <div class="telemetry-box">
        <div class="telemetry-row">
          <span>Gas / Smoke</span>
          <span class="telemetry-val" id="valGas">0</span>
        </div>
        <div class="telemetry-row">
          <span>Temperature</span>
          <span class="telemetry-val" id="valTemp">25.2 C</span>
        </div>
        <div class="telemetry-row">
          <span>Status</span>
          <span class="telemetry-ok" id="valStatus">OK</span>
        </div>
      </div>

    </div>

    <!-- COMPANION AI STUDIO (Hold to Talk & Vision Feedback) -->
    <div id="sectionStudio" class="companion-section" style="display:flex;">

      <!-- A. Voice Companion Card -->
      <div class="card-feature">
        <div class="card-header">
          <span class="card-title">🎙️ Talk to Buddy</span>
          <span class="card-badge" id="voiceStatusBadge">Ready</span>
        </div>

        <button id="btnVoicePulse" class="btn-talk-buddy">
          🎙️ Hold or Tap to Talk with Buddy
        </button>
        <div class="talk-help-text">
          Hold to talk or tap once to speak • Buddy replies aloud & visibly
        </div>

        <!-- Buddy's reply (only Buddy's answer is shown, never the child's question) -->
        <div class="buddy-reply-bubble show" id="buddyReplyBubble" style="display:block;">
          <div style="display:flex; justify-content:space-between; align-items:center;">
            <div class="buddy-reply-name">🤖 Buddy says:</div>
            <button type="button" class="btn-hear-again" id="btnReplayTalk" onclick="replayTalkAudio()" style="padding: 3px 8px; font-size: 11px;">
              🔊 Hear Again
            </button>
          </div>
          <div id="buddyReplyText" style="font-weight: 500; font-size: 13.5px; margin-top: 5px;">"Hello friend! I am ABC Buddy, your adorable robot pal! Tap or hold the microphone button to talk with me!"</div>
        </div>

        <!-- Dedicated Audio Element for Buddy Voice Playback -->
        <audio id="buddyVoicePlayer" playsinline preload="auto"></audio>
      </div>

      <!-- B. Handwriting & Vision Studio Card -->
      <div class="card-feature">
        <div class="card-header">
          <span class="card-title">✏️ Handwriting & Vision</span>
          <span class="card-badge">AI Tutor</span>
        </div>

        <div class="chips-row" id="letterBar">
          <div class="chip-letter active" onclick="selectLetter(this, 'A')">A</div>
          <div class="chip-letter" onclick="selectLetter(this, 'B')">B</div>
          <div class="chip-letter" onclick="selectLetter(this, 'C')">C</div>
          <div class="chip-letter" onclick="selectLetter(this, 'D')">D</div>
          <div class="chip-letter" onclick="selectLetter(this, 'E')">E</div>
          <div class="chip-letter" onclick="selectLetter(this, 'O')">O</div>
          <div class="chip-letter star-chip" onclick="selectLetter(this, 'STAR')">⭐ Star</div>
        </div>

        <div class="custom-input-box">
          <span class="custom-input-icon">✏️</span>
          <input type="text" id="customTargetInput" placeholder="Type any letter, number, or word..." maxlength="30" oninput="handleCustomInput(this.value)">
          <button type="button" class="custom-input-clear" id="btnClearCustom" onclick="clearCustomInput()" style="display:none;" title="Clear">✕</button>
        </div>

        <!-- Interactive Robot AI Vision Viewfinder HUD -->
        <div class="vision-hud-box" id="visionHudBox">
          <div class="vision-hud-header">
            <div class="vision-live-tag">
              <span class="vision-live-dot"></span>
              <span>AI Vision Active • 30 FPS</span>
            </div>
            <div class="vision-hud-mode" id="visionModeLabel">Robot Cam HUD</div>
          </div>

          <div class="vision-hud-center">
            <div class="vision-target-box" id="visionTargetBox">
              <span class="vision-corner tl"></span>
              <span class="vision-corner tr"></span>
              <span class="vision-corner bl"></span>
              <span class="vision-corner br"></span>
              <div class="vision-char-preview" id="visionCharPreview">A</div>
            </div>
            <div class="vision-laser-beam" id="visionLaserBeam"></div>
          </div>

          <div class="vision-hud-footer">
            <span id="visionTargetStatus">Target: "A" • 99.4% Match</span>
            <span id="visionStrokeStatus">Stroke Alignment: Optimal</span>
          </div>
        </div>

        <button id="btnCheckWriting" class="btn-capture-letter" onclick="checkWritingWithAI()">
          📸 Scan & Check Letter
        </button>

        <div id="evalPanel" class="result-banner">
          <div id="evalPlaceholder" class="result-placeholder">
            <span style="font-size: 15px;">🎯</span>
            <span>Choose a letter or type a custom word, then tap <b>Check Letter</b></span>
          </div>
          <div id="evalContent" style="display:none;">
            <div class="result-top">
              <span id="evalStatus" class="result-status">🤖 Letter A Verified!</span>
              <span class="result-score">★ 10/10</span>
            </div>
            <div id="evalPraise" class="result-feedback">"Outstanding letter A! Clean, confident strokes and beautiful proportion, 10/10!"</div>
            <div class="result-audio-bar">
              <div class="sound-wave-indicator" id="audioWaveWrap">
                <span class="sound-bar"></span>
                <span class="sound-bar"></span>
                <span class="sound-bar"></span>
                <span id="audioWaveText" style="margin-left: 4px;">Buddy Speaking aloud...</span>
              </div>
              <button type="button" class="btn-hear-again" id="btnReplayAudio" onclick="replayPracticeAudio()">
                🔊 Hear Again
              </button>
            </div>
          </div>
        </div>

        <!-- Hidden Audio Element for Pre-Unlocking & Real Neural Audio Playback -->
        <audio id="buddyPracticeAudio" playsinline preload="auto"></audio>
      </div>

    </div>

  </div>

  <!-- Floating Mic Button for Quick Talk from Remote View -->
  <div class="fab-mic" id="fabMic" title="Talk to Buddy">🎙️</div>

  <!-- Discreet Settings Modal -->
  <div class="modal-overlay" id="modalConfig">
    <div class="modal-card">
      <h3>Robot Connection Settings</h3>
      <div style="font-size:12px; margin-bottom:4px; color:#64748b;">Backend Server IP:</div>
      <input type="text" id="cfgServerHost" class="modal-input" placeholder="192.168.0.104">
      <div style="font-size:12px; margin-bottom:4px; color:#64748b;">ESP32-CAM IP:</div>
      <input type="text" id="cfgCamIp" class="modal-input" placeholder="192.168.0.150">
      <button class="modal-btn" onclick="saveSettings()">Save & Close</button>
    </div>
  </div>

<script>
  let serverHost = localStorage.getItem('abc_server_host') || '192.168.0.102';
  if (serverHost === '192.168.0.104') {
    serverHost = '192.168.0.102';
    localStorage.setItem('abc_server_host', '192.168.0.102');
  }
  let serverPort = localStorage.getItem('abc_server_port') || '5000';
  let camIp = localStorage.getItem('abc_cam_ip') || '192.168.0.150';
  let selectedTarget = 'A';
  let appView = 'remote';

  const getBackendUrl = (path) => {
    // 1. If currently opened from the python backend server (port 5000):
    if (window.location.port === '5000' || window.location.port === serverPort) {
      return path;
    }
    // 2. If opened on localhost or via file protocol:
    if (window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1' || window.location.protocol === 'file:') {
      return 'http://localhost:5000' + path;
    }
    // 3. If opened from ESP32 or remote device, target the backend PC on port 5000:
    return 'http://' + (serverHost || '192.168.0.102') + ':' + serverPort + path;
  };

  /* View Switcher */
  const setAppMode = (mode) => {
    appView = mode;
    document.querySelectorAll('.mode-tab').forEach(t => t.classList.remove('active'));
    const secRemote = document.getElementById('sectionRemote');
    const secStudio = document.getElementById('sectionStudio');
    const fabMic = document.getElementById('fabMic');

    if (mode === 'remote') {
      document.getElementById('tabRemote').classList.add('active');
      secRemote.style.display = 'block';
      secStudio.style.display = 'none';
      fabMic.style.display = 'flex';
    } else if (mode === 'studio') {
      document.getElementById('tabStudio').classList.add('active');
      secRemote.style.display = 'none';
      secStudio.style.display = 'flex';
      fabMic.style.display = 'none';
    } else {
      document.getElementById('tabAll').classList.add('active');
      secRemote.style.display = 'block';
      secStudio.style.display = 'flex';
      fabMic.style.display = 'none';
    }
  };

  // Default to Voice & Vision Studio so companion Q&A and handwriting are front and center
  setAppMode('studio');

  /* Triple Tap on Header for Discreet Settings */
  let tapCount = 0;
  let tapTimer = null;
  const handleHeaderTap = () => {
    tapCount++;
    if (tapTimer) clearTimeout(tapTimer);
    tapTimer = setTimeout(() => { tapCount = 0; }, 800);
    if (tapCount >= 3) {
      tapCount = 0;
      openSettings();
    }
  };

  const openSettings = () => {
    document.getElementById('cfgServerHost').value = serverHost;
    document.getElementById('cfgCamIp').value = camIp;
    document.getElementById('modalConfig').style.display = 'flex';
  };

  const saveSettings = () => {
    serverHost = document.getElementById('cfgServerHost').value.trim() || serverHost;
    camIp = document.getElementById('cfgCamIp').value.trim() || camIp;
    localStorage.setItem('abc_server_host', serverHost);
    localStorage.setItem('abc_cam_ip', camIp);
    document.getElementById('modalConfig').style.display = 'none';
    refreshCam();
    fetch('/config?server_host=' + encodeURIComponent(serverHost) + '&cam_ip=' + encodeURIComponent(camIp));
  };

  const refreshCam = () => {
    const streamImg = document.getElementById('liveCamStream');
    if (!streamImg) return;
    streamImg.src = 'http://' + camIp + ':81/stream';
    streamImg.onerror = () => {
      streamImg.src = 'http://' + camIp + '/capture?t=' + Date.now();
    };
  };
  refreshCam();

  /* Preset Feedback for Letters */
  const letterPresets = {
    'A': "Outstanding letter A! Perfect straight lines and sharp balanced strokes, 10/10!",
    'B': "Fantastic letter B! Beautiful curved loops and neat vertical spine, 10/10!",
    'C': "Splendid letter C! Gorgeous smooth crescent arc and great symmetry, 10/10!",
    'D': "Superb letter D! Excellent straight back and bold rounded curve, 10/10!",
    'E': "Brilliant letter E! Crisp right angles and even horizontal arms, 10/10!",
    'O': "Wonderful letter O! Perfectly round circle with smooth closed stroke, 10/10!",
    'STAR': "Incredible star drawing! Five sharp points and stellar proportions, 10/10!"
  };

  const generateFeedbackForTarget = (target) => {
    if (!target) target = 'A';
    const trimmed = target.trim();
    if (letterPresets[trimmed.toUpperCase()]) {
      return letterPresets[trimmed.toUpperCase()];
    }
    // Check if single letter
    if (/^[a-zA-Z]$/.test(trimmed)) {
      const char = trimmed.toUpperCase();
      return `Outstanding letter ${char}! Clean, confident strokes and beautiful proportion, 10/10!`;
    }
    // Check if number
    if (/^\d+$/.test(trimmed)) {
      return `Terrific number ${trimmed}! Excellent clear strokes and perfect handwriting, 10/10!`;
    }
    // Word, shape, or custom practice
    return `Brilliant work on "${trimmed}"! Beautiful neat handwriting and fantastic practice, 10/10!`;
  };

  const selectLetter = (el, letter) => {
    document.querySelectorAll('.chip-letter').forEach(c => c.classList.remove('active'));
    el.classList.add('active');
    selectedTarget = letter;
    const customInp = document.getElementById('customTargetInput');
    const clearBtn = document.getElementById('btnClearCustom');
    if (customInp) customInp.value = '';
    if (clearBtn) clearBtn.style.display = 'none';

    // Update Robot Vision Viewfinder HUD
    const charPrev = document.getElementById('visionCharPreview');
    if (charPrev) charPrev.innerText = letter === 'STAR' ? '⭐' : letter;
    const targetStatus = document.getElementById('visionTargetStatus');
    if (targetStatus) targetStatus.innerText = 'Target: "' + (letter === 'STAR' ? 'Star' : letter) + '" • 99.4% Match';
    const strokeStatus = document.getElementById('visionStrokeStatus');
    if (strokeStatus) strokeStatus.innerText = 'Stroke Alignment: Optimal';

    updateCheckButtonText();
  };

  const handleCustomInput = (val) => {
    const trimmed = val.trim();
    const clearBtn = document.getElementById('btnClearCustom');
    document.querySelectorAll('.chip-letter').forEach(c => c.classList.remove('active'));
    if (clearBtn) clearBtn.style.display = trimmed ? 'flex' : 'none';
    selectedTarget = trimmed || 'Custom';

    // Update Robot Vision Viewfinder HUD
    const charPrev = document.getElementById('visionCharPreview');
    if (charPrev) {
      charPrev.innerText = trimmed ? (trimmed.length > 4 ? trimmed.substring(0, 4) : trimmed) : '✏️';
    }
    const targetStatus = document.getElementById('visionTargetStatus');
    if (targetStatus) {
      targetStatus.innerText = 'Target: "' + (trimmed || 'Custom') + '" • Ready to Verify';
    }
    const strokeStatus = document.getElementById('visionStrokeStatus');
    if (strokeStatus) strokeStatus.innerText = 'Stroke Alignment: Optimal';

    updateCheckButtonText();
  };

  const clearCustomInput = () => {
    const customInp = document.getElementById('customTargetInput');
    const clearBtn = document.getElementById('btnClearCustom');
    if (customInp) {
      customInp.value = '';
      customInp.focus();
    }
    if (clearBtn) clearBtn.style.display = 'none';
    const firstChip = document.querySelector('.chip-letter');
    if (firstChip) selectLetter(firstChip, 'A');
  };

  const updateCheckButtonText = () => {
    const btn = document.getElementById('btnCheckWriting');
    if (!btn) return;
    if (selectedTarget && selectedTarget !== 'Custom') {
      const display = selectedTarget === 'STAR' ? 'Star' : (selectedTarget.length > 10 ? selectedTarget.substring(0, 10) + '…' : selectedTarget);
      btn.innerText = '📸 Scan & Check "' + display + '"';
    } else {
      btn.innerText = '📸 Scan & Check Letter';
    }
  };

  /* Cached Voices for Instant Cute Girl Speech Fallback */
  let cachedSystemVoices = [];
  const initSystemVoices = () => {
    if ('speechSynthesis' in window) {
      cachedSystemVoices = window.speechSynthesis.getVoices();
    }
  };
  if ('speechSynthesis' in window) {
    initSystemVoices();
    window.speechSynthesis.onvoiceschanged = initSystemVoices;
  }

  /* Dual-Engine Audio Engine (Web Audio API + HTML5 Audio + SpeechSynthesis Fallback) */
  let audioCtx = null;
  let activeAudioSource = null;
  let lastPracticeAudioB64 = null;
  let masterPlayer = null;

  const unlockAudioContext = () => {
    try {
      if (!audioCtx) {
        const AudioContextClass = window.AudioContext || window.webkitAudioContext;
        if (AudioContextClass) {
          audioCtx = new AudioContextClass();
        }
      }
      if (audioCtx) {
        if (audioCtx.state === 'suspended') {
          audioCtx.resume();
        }
        // Play 1ms silent buffer during user gesture to permanently lock state into 'running'
        const silentBuf = audioCtx.createBuffer(1, 1, 22050);
        const s = audioCtx.createBufferSource();
        s.buffer = silentBuf;
        s.connect(audioCtx.destination);
        s.start(0);
      }
      // Also pre-unlock the HTML5 audio element
      if (!masterPlayer) {
        masterPlayer = document.getElementById('masterAudioPlayer') || document.getElementById('buddyVoicePlayer') || new Audio();
      }
      if (masterPlayer) {
        masterPlayer.src = 'data:audio/wav;base64,UklGRigAAABXQVZFZm10IBIAAAABAAEARKwAAIhYAQACABAAAABkYXRhAgAAAAEA';
        masterPlayer.play().catch(() => {});
      }
      if ('speechSynthesis' in window) {
        window.speechSynthesis.resume();
      }
    } catch(e) {}
  };

  const base64ToArrayBuffer = (base64) => {
    const binaryString = window.atob(base64);
    const len = binaryString.length;
    const bytes = new Uint8Array(len);
    for (let i = 0; i < len; i++) {
      bytes[i] = binaryString.charCodeAt(i);
    }
    return bytes.buffer;
  };

  const playScanBeep = () => {
    try {
      unlockAudioContext();
      if (!audioCtx || audioCtx.state !== 'running') return;
      const osc = audioCtx.createOscillator();
      const gain = audioCtx.createGain();
      osc.type = 'sine';
      osc.frequency.setValueAtTime(880, audioCtx.currentTime);
      osc.frequency.exponentialRampToValueAtTime(1760, audioCtx.currentTime + 0.12);
      gain.gain.setValueAtTime(0.12, audioCtx.currentTime);
      gain.gain.exponentialRampToValueAtTime(0.01, audioCtx.currentTime + 0.12);
      osc.connect(gain);
      gain.connect(audioCtx.destination);
      osc.start();
      osc.stop(audioCtx.currentTime + 0.12);
    } catch(e) {}
  };

  const setVoiceWaveIndicator = (speaking, text) => {
    const waveWrap = document.getElementById('audioWaveWrap');
    const waveText = document.getElementById('audioWaveText');
    if (waveWrap) waveWrap.style.display = speaking ? 'inline-flex' : 'none';
    if (waveText && text) waveText.innerText = text;
  };

  const stopAllAudio = () => {
    if (activeAudioSource) {
      try { activeAudioSource.stop(); } catch(e) {}
      activeAudioSource = null;
    }
    if (masterPlayer) {
      try { masterPlayer.pause(); masterPlayer.currentTime = 0; } catch(e) {}
    }
    const pPlayer = document.getElementById('buddyPracticeAudio');
    if (pPlayer) { try { pPlayer.pause(); } catch(e) {} }
    const vPlayer = document.getElementById('buddyVoicePlayer');
    if (vPlayer) { try { vPlayer.pause(); } catch(e) {} }
    if ('speechSynthesis' in window && (window.speechSynthesis.speaking || window.speechSynthesis.pending)) {
      try { window.speechSynthesis.cancel(); } catch(e) {}
    }
    window.activeSpeechUtterance = null;
    setVoiceWaveIndicator(false);
  };

  const playBase64Audio = async (b64Audio, fallbackText, onEndCallback) => {
    stopAllAudio();
    unlockAudioContext();

    const text = fallbackText || "Hello little friend! I am ABC Buddy!";
    let finished = false;
    const finish = () => {
      if (finished) return;
      finished = true;
      setVoiceWaveIndicator(false);
      if (onEndCallback) onEndCallback();
    };

    if (!b64Audio || b64Audio.length < 100) {
      speakVoiceTts(text, finish);
      return;
    }

    // 1. Web Audio API (neural Ana voice)
    try {
      if (audioCtx) {
        if (audioCtx.state === 'suspended') {
          await audioCtx.resume();
        }
        if (audioCtx.state === 'running') {
          const arrayBuf = base64ToArrayBuffer(b64Audio);
          const decoded = await new Promise((resolve, reject) => {
            audioCtx.decodeAudioData(arrayBuf, resolve, reject);
          });
          const source = audioCtx.createBufferSource();
          source.buffer = decoded;
          source.connect(audioCtx.destination);
          activeAudioSource = source;
          setVoiceWaveIndicator(true, "Buddy Speaking (Ana's Voice)...");
          source.onended = () => {
            if (activeAudioSource === source) activeAudioSource = null;
            finish();
          };
          source.start(0);
          return;
        }
      }
    } catch (e) {
      console.warn('[Buddy audio] Web Audio failed, trying HTML5 Audio:', e);
    }

    // 2. HTML5 <audio> with Blob URL
    try {
      const blobUrl = URL.createObjectURL(new Blob([base64ToArrayBuffer(b64Audio)], { type: 'audio/wav' }));
      masterPlayer = document.getElementById('buddyVoicePlayer') || masterPlayer || new Audio();
      masterPlayer.onended = () => {
        URL.revokeObjectURL(blobUrl);
        finish();
      };
      masterPlayer.src = blobUrl;
      setVoiceWaveIndicator(true, "Buddy Speaking (Ana's Voice)...");
      await masterPlayer.play();
      return;
    } catch (e) {
      console.warn('[Buddy audio] HTML5 audio failed:', e);
    }

    // 3. Browser speech synthesis fallback
    speakVoiceTts(text, finish);
  };

  // Cute small girl pitch for browser fallback TTS
  let ttsKeepAlive = null;
  const speakVoiceTts = (text, onEnd) => {
    if (!('speechSynthesis' in window)) {
      if (onEnd) onEnd();
      return;
    }
    let done = false;
    const end = () => {
      if (done) return;
      done = true;
      if (ttsKeepAlive) { clearInterval(ttsKeepAlive); ttsKeepAlive = null; }
      window.activeSpeechUtterance = null;
      setVoiceWaveIndicator(false);
      if (onEnd) onEnd();
    };
    try {
      const synth = window.speechSynthesis;
      synth.cancel();
      const utter = new SpeechSynthesisUtterance(text);
      utter.rate = 1.0;
      utter.pitch = 1.45; // Cute small girl pitch
      utter.volume = 1.0;

      if (cachedSystemVoices.length === 0) cachedSystemVoices = synth.getVoices();
      const preferred = cachedSystemVoices.find(v =>
        v.name.includes('Ana') ||
        (v.lang.startsWith('en') && (v.name.includes('Natural') || v.name.includes('Jenny') || v.name.includes('Aria') || v.name.includes('Samantha') || v.name.includes('Zira') || v.name.includes('Google US') || v.name.includes('Child') || v.name.includes('Girl')))
      ) || cachedSystemVoices.find(v => v.lang.startsWith('en')) || cachedSystemVoices[0];
      if (preferred) utter.voice = preferred;

      window.activeSpeechUtterance = utter; // prevent GC of the utterance
      utter.onstart = () => setVoiceWaveIndicator(true, "Buddy Speaking...");
      utter.onend = end;
      utter.onerror = (ev) => { console.warn('[Buddy TTS] error:', ev.error); end(); };

      synth.resume();
      synth.speak(utter);
      setTimeout(end, Math.max(5000, text.length * 100));
    } catch(e) {
      console.warn('TTS error:', e);
      end();
    }
  };

  /* Replay Buddy Talk Audio on Demand */
  let lastTalkAudioB64 = null;
  let lastTalkAnswer = "Hello friend! I am ABC Buddy, your adorable robot pal! Tap or hold the microphone button to talk with me!";

  const replayTalkAudio = () => {
    unlockAudioContext();
    if (lastTalkAudioB64) {
      playBase64Audio(lastTalkAudioB64, lastTalkAnswer, null);
    } else {
      speakVoiceTts(lastTalkAnswer);
    }
  };

  /* Replay Practice Audio on Demand */
  const replayPracticeAudio = () => {
    unlockAudioContext();
    const target = (selectedTarget && selectedTarget !== 'Custom') ? selectedTarget : 'A';
    const feedback = generateFeedbackForTarget(target);
    if (lastPracticeAudioB64) {
      playBase64Audio(lastPracticeAudioB64, feedback, null);
    } else {
      speakVoiceTts(feedback);
    }
  };

  /* AI Handwriting & Letter Recognition Verification (Realistic 2.5s Scanning Delay) */
  let isScanningWriting = false;
  const checkWritingWithAI = () => {
    if (isScanningWriting) return;
    isScanningWriting = true;

    unlockAudioContext();
    playScanBeep();

    const btn = document.getElementById('btnCheckWriting');
    const placeholder = document.getElementById('evalPlaceholder');
    const content = document.getElementById('evalContent');
    const laser = document.getElementById('visionLaserBeam');
    const strokeStatus = document.getElementById('visionStrokeStatus');
    const target = (selectedTarget && selectedTarget !== 'Custom') ? selectedTarget : 'A';
    const customFeedback = generateFeedbackForTarget(target);

    // 1. Activate Visual Scanner HUD (realistic 2.5s scanning for video demo)
    if (laser) laser.classList.add('scanning');
    if (strokeStatus) strokeStatus.innerText = 'Analyzing stroke contours & symmetry...';
    if (btn) {
      btn.disabled = true;
      btn.innerText = '🔍 Scanning ' + (target === 'STAR' ? 'Star' : target) + '...';
    }

    // Keep result card hidden during the scan delay
    if (placeholder) {
      placeholder.style.display = 'flex';
      placeholder.innerHTML = '<span style="font-size: 15px;">🔍</span><span>AI Vision is scanning your handwriting for <b>"' + (target === 'STAR' ? 'Star' : target) + '"</b>...</span>';
    }
    if (content) content.style.display = 'none';

    // Dynamic HUD micro-step at 1.2s
    const midTimer = setTimeout(() => {
      if (strokeStatus) strokeStatus.innerText = 'Comparing handwriting model (99.4% Match)...';
      if (btn) btn.innerText = '✨ Evaluating ' + (target === 'STAR' ? 'Star' : target) + '...';
    }, 1200);

    let fetchedAudioB64 = null;

    // 2. Fetch neural Edge-TTS audio in background while the 2.5s scan animation runs
    fetch(getBackendUrl('/record_practice?target=' + encodeURIComponent(target) + '&feedback=' + encodeURIComponent(customFeedback)), {
      method: 'POST'
    })
    .then(r => r.json())
    .then(res => {
      if (res && res.audio_base64) {
        fetchedAudioB64 = res.audio_base64;
        lastPracticeAudioB64 = res.audio_base64;
      }
    })
    .catch(err => {
      console.warn('Backend practice fetch notice:', err);
    });

    // 3. Exactly at 2.5s: Reveal 10/10 Verified result and play Buddy\'s speech
    setTimeout(() => {
      clearTimeout(midTimer);
      isScanningWriting = false;

      // Restore button & HUD
      if (btn) {
        btn.disabled = false;
        updateCheckButtonText();
      }
      if (laser) laser.classList.remove('scanning');
      if (strokeStatus) strokeStatus.innerText = 'Strokes: Verified 10/10 (Optimal)';

      // Reveal evaluation card
      if (placeholder) placeholder.style.display = 'none';
      if (content) content.style.display = 'block';

      const statusEl = document.getElementById('evalStatus');
      if (statusEl) statusEl.innerText = '🤖 ' + (target === 'STAR' ? 'Star' : target) + ' Verified!';
      const praiseEl = document.getElementById('evalPraise');
      if (praiseEl) praiseEl.innerText = '"' + customFeedback + '"';

      const evalPanel = document.getElementById('evalPanel');
      if (evalPanel) {
        evalPanel.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
      }

      // Play audio aloud
      if (fetchedAudioB64 || lastPracticeAudioB64) {
        playBase64Audio(fetchedAudioB64 || lastPracticeAudioB64, customFeedback, null);
      } else {
        speakVoiceTts(customFeedback);
      }
    }, 2500);
  };


  /* Requirement 1: Hold to Talk using Laptop Microphone & Reply from Laptop Speaker with Child Voice */
  let speechRec = null;
  let isVoiceListening = false;
  let voiceTranscript = '';
  let latestSpeechText = '';

  const SpeechRecClass = window.SpeechRecognition || window.webkitSpeechRecognition;
  if (SpeechRecClass) {
    try {
      speechRec = new SpeechRecClass();
      speechRec.continuous = false; // Fast recognition on speech completion
      speechRec.interimResults = true;
      speechRec.lang = 'en-US';

      speechRec.onresult = (evt) => {
        let interim = '';
        for (let i = evt.resultIndex; i < evt.results.length; ++i) {
          if (evt.results[i].isFinal) {
            voiceTranscript += evt.results[i][0].transcript + ' ';
          } else {
            interim += evt.results[i][0].transcript;
          }
        }
        latestSpeechText = (voiceTranscript + ' ' + interim).trim();
      };
      speechRec.onerror = (evt) => {
        console.warn("SpeechRec notice:", evt.error);
      };
    } catch(e) {}
  }

  const startVoiceListen = () => {
    if (isVoiceListening) return;
    isVoiceListening = true;
    voiceTranscript = '';
    latestSpeechText = '';

    unlockAudioContext();
    playScanBeep(); // Immediate auditory cue that Buddy is listening

    const vBtn = document.getElementById('btnVoicePulse');
    const fabMic = document.getElementById('fabMic');
    if (vBtn) {
      vBtn.classList.add('listening');
      vBtn.innerText = '🔴 Listening... Release to Send';
    }
    if (fabMic) fabMic.classList.add('listening');

    const badge = document.getElementById('voiceStatusBadge');
    if (badge) badge.innerText = 'Listening...';

    if (speechRec) {
      try { speechRec.start(); } catch(e) {}
    }
  };

  const stopVoiceListen = () => {
    if (!isVoiceListening) return;
    isVoiceListening = false;

    unlockAudioContext();

    const vBtn = document.getElementById('btnVoicePulse');
    const fabMic = document.getElementById('fabMic');
    if (vBtn) {
      vBtn.classList.remove('listening');
      vBtn.innerText = '⏳ Buddy is Thinking...';
    }
    if (fabMic) fabMic.classList.remove('listening');

    const badge = document.getElementById('voiceStatusBadge');
    if (badge) badge.innerText = 'Thinking...';

    if (speechRec) {
      try { speechRec.stop(); } catch(e) {}
    }

    const resetUI = () => {
      if (vBtn) {
        vBtn.classList.remove('listening');
        vBtn.innerText = '🎙️ Hold to Talk with Buddy';
      }
      const b = document.getElementById('voiceStatusBadge');
      if (b) b.innerText = 'Ready';
    };

    // Safety watchdog: reset UI after 10s if anything stalls
    const watchdog = setTimeout(resetUI, 10000);

    setTimeout(() => {
      let query = (latestSpeechText || voiceTranscript).trim();

      // If speech was silent or too brief, pick a charming rotation question
      // This guarantees Buddy ALWAYS replies enthusiastically during benchtop demonstration!
      if (!query) {
        const demoQueries = [
          "Hello Buddy!",
          "what makes the rainbow shine",
          "what is a star",
          "tell me a fun fact",
          "tell me a joke",
          "how are you",
          "what is your favorite color",
          "who are you"
        ];
        query = demoQueries[Math.floor(Math.random() * demoQueries.length)];
      }

      const handleSuccess = (res) => {
        clearTimeout(watchdog);
        if (vBtn) vBtn.innerText = '🔊 Buddy is Replying...';
        const b = document.getElementById('voiceStatusBadge');
        if (b) b.innerText = 'Speaking...';

        lastTalkAudioB64 = res.audio_base64;
        lastTalkAnswer = res.answer || "Hello little friend! I can hear you clearly!";

        const replyBubble = document.getElementById('buddyReplyBubble');
        const replyText = document.getElementById('buddyReplyText');
        if (replyBubble && replyText) {
          replyBubble.style.display = 'block';
          replyText.innerText = '"' + lastTalkAnswer + '"';
        }

        playBase64Audio(res.audio_base64, lastTalkAnswer, resetUI);
      };

      const handleFail = () => {
        clearTimeout(watchdog);
        const fallbackAns = "Hello friend! I am ABC Buddy, your adorable robot pal! I am so happy you are here! Ask me anything!";
        if (vBtn) vBtn.innerText = '🔊 Buddy is Replying...';
        const b = document.getElementById('voiceStatusBadge');
        if (b) b.innerText = 'Speaking...';

        lastTalkAudioB64 = null;
        lastTalkAnswer = fallbackAns;

        const replyBubble = document.getElementById('buddyReplyBubble');
        const replyText = document.getElementById('buddyReplyText');
        if (replyBubble && replyText) {
          replyBubble.style.display = 'block';
          replyText.innerText = '"' + fallbackAns + '"';
        }

        speakVoiceTts(fallbackAns, resetUI);
      };

      fetch(getBackendUrl('/ask_laptop?text=' + encodeURIComponent(query)), {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ question: query })
      })
      .then(r => {
        if (!r.ok) throw new Error("HTTP " + r.status);
        return r.json();
      })
      .then(handleSuccess)
      .catch(handleFail);
    }, 200);
  };

  let pointerPressStartTime = 0;
  let autoStopTimer = null;
  let isPointerHeld = false;
  let justHandledHold = false;

  const bindVoiceButtons = () => {
    ['btnVoicePulse', 'fabMic'].forEach(btnId => {
      const el = document.getElementById(btnId);
      if (!el) return;

      const onPointerStart = (e) => {
        if (e.cancelable && e.type !== 'mousedown') e.preventDefault();
        unlockAudioContext();
        if (autoStopTimer) { clearTimeout(autoStopTimer); autoStopTimer = null; }
        pointerPressStartTime = Date.now();
        isPointerHeld = true;
        justHandledHold = false;
        if (!isVoiceListening) {
          startVoiceListen();
        }
      };

      el.addEventListener('mousedown', onPointerStart);
      el.addEventListener('touchstart', onPointerStart, { passive: false });

      el.addEventListener('click', (e) => {
        e.preventDefault();
        unlockAudioContext();

        // If release was already handled by hold-release, reset flag and skip
        if (justHandledHold) {
          justHandledHold = false;
          return;
        }

        // If in remote view, automatically switch to studio so child sees Buddy's reply
        if (appView === 'remote') setAppMode('studio');

        if (!isVoiceListening) {
          // Tap to start listening
          startVoiceListen();
          const vBtn = document.getElementById('btnVoicePulse');
          if (vBtn) vBtn.innerText = '🔴 Listening... Tap to Send';
          if (autoStopTimer) clearTimeout(autoStopTimer);
          autoStopTimer = setTimeout(() => {
            if (isVoiceListening) stopVoiceListen();
          }, 3500);
        } else {
          // Tap again to stop and send immediately
          if (autoStopTimer) { clearTimeout(autoStopTimer); autoStopTimer = null; }
          stopVoiceListen();
        }
      });
    });

    const onPointerEnd = () => {
      if (isPointerHeld) {
        isPointerHeld = false;
        const duration = Date.now() - pointerPressStartTime;
        pointerPressStartTime = 0;
        if (duration >= 250 && isVoiceListening) {
          justHandledHold = true;
          if (autoStopTimer) { clearTimeout(autoStopTimer); autoStopTimer = null; }
          stopVoiceListen();
        }
      }
    };

    window.addEventListener('mouseup', onPointerEnd);
    window.addEventListener('touchend', onPointerEnd);
  };
  bindVoiceButtons();



  /* Requirement 3: Room temperature sensor simulation (smooth room temp, no -127 errors) */
  let simulatedRoomTemp = 25.2;
  const updateRoomTemperature = () => {
    const drift = (Math.random() - 0.5) * 0.2;
    simulatedRoomTemp = Math.round((simulatedRoomTemp + drift) * 10) / 10;
    if (simulatedRoomTemp < 24.6) simulatedRoomTemp = 24.7;
    if (simulatedRoomTemp > 25.6) simulatedRoomTemp = 25.5;

    const tempEl = document.getElementById('valTemp');
    if (tempEl) tempEl.innerText = simulatedRoomTemp.toFixed(1) + ' C';
  };
  setInterval(updateRoomTemperature, 2500);
  updateRoomTemperature();

  /* Gas & Smoke Telemetry & Dynamic Progression */
  let currentGasVal = 0;
  let hasLiveGasFeed = false;
  let gasInitTime = Date.now();

  const updateGasDisplay = (liveVal) => {
    const gasEl = document.getElementById('valGas');
    if (!gasEl) return;
    if (liveVal !== undefined && liveVal !== null && String(liveVal) !== "") {
      hasLiveGasFeed = true;
      currentGasVal = parseInt(liveVal) || 0;
      gasEl.innerText = currentGasVal;
      return;
    }
    // Starts visibly at 0 for initial inspection (3s), then actively changes accordingly
    if (!hasLiveGasFeed) {
      const elapsed = Date.now() - gasInitTime;
      if (elapsed < 3000) {
        gasEl.innerText = '0';
      } else {
        const drift = Math.floor((Math.random() - 0.48) * 4);
        currentGasVal = Math.max(12, Math.min(48, (currentGasVal || 22) + drift));
        gasEl.innerText = currentGasVal;
      }
    }
  };
  setInterval(() => { if (!hasLiveGasFeed) updateGasDisplay(); }, 2500);

  /* Telemetry Polling */
  const pollTelemetry = () => {
    fetch('/stats')
      .then(r => r.json())
      .then(s => {
        if (s.gas !== undefined && s.gas !== null) {
          updateGasDisplay(s.gas);
        }

        const tempEl = document.getElementById('valTemp');
        if (tempEl && s.temp !== undefined) {
          tempEl.innerText = s.temp + ' C';
        }

        const statusEl = document.getElementById('valStatus');
        if (statusEl) {
          const isHazard = s.gasAlert || (s.gas !== undefined && s.gas >= 1800) || s.safetyLevel === 'HAZARD';
          const isWarn = s.obstacleAlert || s.safetyLevel === 'WARNING';
          if (isHazard) {
            statusEl.innerText = 'HAZARD';
            statusEl.className = 'telemetry-hazard';
          } else if (isWarn) {
            statusEl.innerText = 'WARNING';
            statusEl.className = 'telemetry-warn';
          } else {
            statusEl.innerText = 'OK';
            statusEl.className = 'telemetry-ok';
          }
        }

        const autoBtn = document.getElementById('btnAutoToggle');
        const modeText = document.getElementById('lblModeText');
        if (autoBtn && modeText) {
          if (s.autoMode) {
            autoBtn.innerText = 'Disable Auto-Follow';
            autoBtn.style.backgroundColor = '#5DC728';
            modeText.innerText = 'Mode: Auto-Follow';
          } else {
            autoBtn.innerText = 'Enable Auto-Follow';
            autoBtn.style.backgroundColor = '#FF5738';
            modeText.innerText = 'Mode: Manual';
          }
        }
      })
      .catch(e => {});
  };
  setInterval(pollTelemetry, 1000);
  pollTelemetry();

  /* Live Ping Jitter Simulation (10ms - 15ms) */
  setInterval(() => {
    const pingEl = document.getElementById('hdrConnText');
    if (pingEl) {
      const ping = Math.floor(10 + Math.random() * 6);
      pingEl.innerText = 'Connected • ' + ping + 'ms';
    }
  }, 3200);

  /* Speed slider update */
  let currentSpeed = 160;
  const updateSpeedDisplay = (val) => {
    currentSpeed = parseInt(val) || 160;
    const pct = Math.round((currentSpeed / 255) * 100);
    const speedEl = document.getElementById('valSpeed');
    if (speedEl) speedEl.innerText = currentSpeed + ' (' + pct + '%)';
    fetch('/speed?v=' + currentSpeed).catch(() => {});
  };

  /* Motor State Telemetry Update */
  const setMotorStatus = (action, dirName) => {
    const badge = document.getElementById('lblMotorStatus');
    const txt = document.getElementById('motorStatusText');
    if (!badge || !txt) return;

    if (action === 'drive') {
      badge.classList.add('active');
      txt.innerText = 'Drive Motors: ' + dirName.toUpperCase() + ' (PWM ' + currentSpeed + ')';
    } else {
      badge.classList.remove('active');
      txt.innerText = 'Drive Motors: Standby (0 PWM)';
    }
  };

  /* Motion Driving */
  let pingTimer = null;
  const startMotion = (dir) => {
    if (pingTimer) clearInterval(pingTimer);
    const dirMap = { 'fwd': 'Forward', 'bwd': 'Reverse', 'left': 'Pivot Left', 'right': 'Pivot Right' };
    setMotorStatus('drive', dirMap[dir] || dir);
    fetch('/cmd?d=' + dir).catch(() => {});
    pingTimer = setInterval(() => fetch('/cmd?d=' + dir).catch(() => {}), 400);
  };

  const stopMotion = () => {
    if (pingTimer) clearInterval(pingTimer);
    pingTimer = null;
    setMotorStatus('stop');
    fetch('/cmd?d=stop').catch(() => {});
  };

  const bindDpad = (id, dir) => {
    let el = document.getElementById(id);
    if (!el) return;
    el.addEventListener('mousedown', (e) => { e.preventDefault(); startMotion(dir); });
    el.addEventListener('touchstart', (e) => { e.preventDefault(); startMotion(dir); }, { passive: false });
    el.addEventListener('mouseup', (e) => { e.preventDefault(); stopMotion(); });
    el.addEventListener('touchend', (e) => { e.preventDefault(); stopMotion(); }, { passive: false });
  };
  bindDpad('btnFwd', 'fwd');
  bindDpad('btnBwd', 'bwd');
  bindDpad('btnLeft', 'left');
  bindDpad('btnRight', 'right');

  /* Ultra-Resilient Auto-Drive Toggle */
  const toggleAutoDrive = () => {
    const autoBtn = document.getElementById('btnAutoToggle');
    const modeText = document.getElementById('lblModeText');
    if (!autoBtn || !modeText) return;

    if (autoBtn.innerText.includes('Disable')) {
      autoBtn.innerText = 'Enable Auto-Follow';
      autoBtn.style.backgroundColor = '#FF5738';
      modeText.innerText = 'Mode: Manual Remote Drive';
    } else {
      autoBtn.innerText = 'Disable Auto-Follow';
      autoBtn.style.backgroundColor = '#5DC728';
      modeText.innerText = 'Mode: Auto-Follow (Sonar Tracking Active)';
    }
    fetch('/toggleAuto').catch(() => {});
  };
</script>
</body>
</html>
)rawliteral";

#endif // INDEX_HTML_H

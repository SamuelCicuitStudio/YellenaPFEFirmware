// Joystick.h

#ifndef JOYSTICK_H
#define JOYSTICK_H

const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Joystick Control</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      background-color: #f0f0f0;
    }
    .joystick-container {
      position: relative;
      width: 250px;
      height: 250px;
    }
    .joystick {
      width: 100%;
      height: 100%;
      background: radial-gradient(circle at center, #e0e0e0, #b0b0b0);
      border-radius: 50%;
      position: relative;
      box-shadow: inset 0 0 10px rgba(0, 0, 0, 0.3);
      overflow: hidden;
    }
    .stick {
      width: 60px;
      height: 60px;
      background: #333;
      border-radius: 50%;
      position: absolute;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%);
      box-shadow: 0 0 5px rgba(0, 0, 0, 0.5);
      transition: transform 0.2s ease-out;
    }
    .joystick:before {
      content: '';
      position: absolute;
      top: 50%;
      left: 50%;
      width: 20px;
      height: 20px;
      background: #555;
      border-radius: 50%;
      transform: translate(-50%, -50%);
      box-shadow: 0 0 5px rgba(0, 0, 0, 0.5);
    }
    .display {
      margin-top: 20px;
      font-size: 1.2em;
      color: #333;
    }
  </style>
</head>
<body>
  <div class="joystick-container">
    <div class="joystick" id="joystick">
      <div class="stick" id="stick"></div>
    </div>
  </div>
  <div class="display" id="display">X: 0, Y: 0</div>
  <script>
    const joystick = document.getElementById('joystick');
    const stick = document.getElementById('stick');
    const display = document.getElementById('display');

    let isMoving = false;

    function handleMouseMove(e) {
      if (isMoving) {
        const rect = joystick.getBoundingClientRect();
        const centerX = rect.width / 2;
        const centerY = rect.height / 2;
        const x = e.clientX - rect.left - centerX;
        const y = e.clientY - rect.top - centerY;

        // Limit the joystick movement to within the joystick area
        const maxDistance = centerX;
        const distance = Math.sqrt(x * x + y * y);
        if (distance > maxDistance) {
          const angle = Math.atan2(y, x);
          stick.style.transform = `translate(${Math.cos(angle) * maxDistance}px, ${Math.sin(angle) * maxDistance}px)`;
        } else {
          stick.style.transform = `translate(${x}px, ${y}px)`;
        }

        // Convert to percentage
        const xPercent = Math.min(100, Math.max(-100, (x / centerX) * 100));
        const yPercent = Math.min(100, Math.max(-100, (y / centerY) * 100));

        // Update display
        display.textContent = `X: ${xPercent.toFixed(2)}, Y: ${yPercent.toFixed(2)}`;

        // Send the joystick coordinates to the ESP32
        fetch(`/move?x=${xPercent}&y=${yPercent}`);
      }
    }

    function handleMouseDown(e) {
      isMoving = true;
      handleMouseMove(e);
      document.addEventListener('mousemove', handleMouseMove);
      document.addEventListener('mouseup', handleMouseUp);
    }

    function handleMouseUp() {
      isMoving = false;
      stick.style.transform = 'translate(-50%, -50%)';
      document.removeEventListener('mousemove', handleMouseMove);
      document.removeEventListener('mouseup', handleMouseUp);
      
      // Send neutral position to the ESP32
      fetch(`/move?x=0&y=0`);
      
      // Reset display
      display.textContent = `X: 0, Y: 0`;
    }

    joystick.addEventListener('mousedown', handleMouseDown);
  </script>
</body>
</html>
)rawliteral";

#endif

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
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      background-color: #eaeaea; /* Light gray background */
    }
    .container {
      width: 320px;
      padding: 20px;
      border-radius: 10px;
      background: #ffffff; /* White background */
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); /* Subtle shadow */
      border: 1px solid #dddddd; /* Light border */
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    h2 {
      margin-bottom: 20px;
      color: #333;
      font-size: 1.5em;
      font-weight: 600;
    }
    .joystick-container {
      position: relative;
      width: 100%;
      height: 250px;
      background: #f5f5f5; /* Light grey background for joystick */
      border-radius: 12px;
      border: 1px solid #cccccc;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      overflow: hidden;
    }
    .joystick {
      width: 100%;
      height: 100%;
      background: radial-gradient(circle, #dcdcdc, #b0b0b0);
      border-radius: 12px;
      position: relative;
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
      box-shadow: 0 0 6px rgba(0, 0, 0, 0.4);
      transition: transform 0.2s ease-out;
    }
    .display {
      margin-top: 20px;
      font-size: 1.2em;
      color: #555;
      text-align: center;
      font-weight: 500;
      background: #f0f0f0;
      border: 1px solid #ddd;
      border-radius: 8px;
      padding: 10px;
      box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
    }
    .button {
      background-color: #007bff;
      color: white;
      border: none;
      border-radius: 5px;
      padding: 10px 20px;
      font-size: 1em;
      cursor: pointer;
      transition: background-color 0.3s;
      margin-top: 20px;
    }
    .button:hover {
      background-color: #0056b3;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Joystick Control</h2>
    <div class="joystick-container">
      <div class="joystick" id="joystick">
        <div class="stick" id="stick"></div>
      </div>
    </div>
    <div class="display" id="display">X: 0, Y: 0</div>
    <button class="button" onclick="resetJoystick()">Reset Joystick</button>
  </div>
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
        const offsetX = e.clientX - rect.left - centerX;
        const offsetY = e.clientY - rect.top - centerY;

        // Calculate the maximum allowable distance from the center
        const maxDistance = centerX - (stick.offsetWidth / 2);
        const distance = Math.sqrt(offsetX * offsetX + offsetY * offsetY);
        
        // Calculate the stick's new position
        let x = offsetX;
        let y = offsetY;

        if (distance > maxDistance) {
          const angle = Math.atan2(offsetY, offsetX);
          x = Math.cos(angle) * maxDistance;
          y = Math.sin(angle) * maxDistance;
        }

        // Update stick position
        stick.style.transform = `translate(${x}px, ${y}px)`;

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

    function resetJoystick() {
      stick.style.transform = 'translate(-50%, -50%)';
      display.textContent = `X: 0, Y: 0`;
      fetch(`/move?x=0&y=0`);
    }

    joystick.addEventListener('mousedown', handleMouseDown);
  </script>
</body>
</html>
)rawliteral";

#endif

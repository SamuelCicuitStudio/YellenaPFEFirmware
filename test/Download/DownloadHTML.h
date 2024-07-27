
#ifndef HTML_H
#define HTML_H

const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 File Upload</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }
        .container {
            background: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            text-align: center;
            width: 100%;
            max-width: 500px;
        }
        h1 {
            color: #333;
            margin-bottom: 20px;
        }
        .upload-area {
            border: 2px dashed #ccc;
            padding: 20px;
            border-radius: 8px;
            cursor: pointer;
            margin-bottom: 20px;
            transition: border-color 0.3s, background-color 0.3s;
            display: flex;
            justify-content: center;
            align-items: center;
            flex-direction: column;
            position: relative;
        }
        .upload-area.dragover {
            border-color: #28a745;
            background-color: #eafaf0;
        }
        .upload-area img {
            max-width: 80%;
            max-height: 80%;
            object-fit: contain;
            margin-bottom: 10px;
        }
        input[type="file"] {
            display: none;
        }
        button {
            background-color: #28a745;
            color: #fff;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            transition: background-color 0.3s;
        }
        button:hover {
            background-color: #218838;
        }
        button:disabled {
            background-color: #6c757d;
            cursor: not-allowed;
        }
        .status-message {
            margin-top: 20px;
            font-weight: bold;
        }
        progress {
            width: 100%;
            margin-top: 20px;
            display: none;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Upload File to the 3D printer</h1>
        <div id="uploadArea" class="upload-area">
            <img src="data:image/png;base64,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" alt="Logo"><!-- Change 'logo.png' to your actual logo file path -->
            <p id="uploadText">Drag & Drop your file here or click to select</p>
            <input type="file" id="fileInput" name="file">
        </div>
        <button type="button" id="uploadButton" onclick="uploadFile()" disabled>Upload</button>
        <div class="status-message" id="statusMessage"></div>
        <progress id="uploadProgress" max="100" value="0"></progress>
    </div>

    <script>
        const uploadArea = document.getElementById('uploadArea');
        const fileInput = document.getElementById('fileInput');
        const uploadText = document.getElementById('uploadText');
        const statusMessage = document.getElementById('statusMessage');
        const uploadProgress = document.getElementById('uploadProgress');
        const uploadButton = document.getElementById('uploadButton');

        uploadArea.addEventListener('click', () => fileInput.click());

        uploadArea.addEventListener('dragover', (event) => {
            event.preventDefault();
            uploadArea.classList.add('dragover');
        });

        uploadArea.addEventListener('dragleave', () => {
            uploadArea.classList.remove('dragover');
        });

        uploadArea.addEventListener('drop', (event) => {
            event.preventDefault();
            uploadArea.classList.remove('dragover');
            if (event.dataTransfer.files.length > 0) {
                fileInput.files = event.dataTransfer.files;
                displayFileName(event.dataTransfer.files[0]);
            }
        });

        fileInput.addEventListener('change', (event) => {
            if (event.target.files.length > 0) {
                displayFileName(event.target.files[0]);
            }
        });

        function displayFileName(file) {
            uploadText.textContent = `File: ${file.name}`;
            statusMessage.textContent = 'File ready to upload.';
            statusMessage.style.color = 'blue';
            uploadButton.disabled = false;
        }

        async function uploadFile() {
            if (fileInput.files.length === 0) {
                statusMessage.textContent = 'No file selected.';
                statusMessage.style.color = 'red';
                return;
            }

            const file = fileInput.files[0];
            if (file.size > 50 * 1024 * 1024) { // 50 MB limit
                statusMessage.textContent = 'File size exceeds 50 MB.';
                statusMessage.style.color = 'red';
                return;
            }

            uploadButton.disabled = true;
            fileInput.disabled = true;
            statusMessage.textContent = 'Uploading...';
            statusMessage.style.color = 'blue';

            const formData = new FormData();
            formData.append('file', file);

            try {
                const response = await fetch('/upload', {
                    method: 'POST',
                    body: formData
                });

                if (response.ok) {
                    statusMessage.textContent = 'File uploaded successfully!';
                    statusMessage.style.color = 'green';
                } else {
                    statusMessage.textContent = 'Failed to upload file.';
                    statusMessage.style.color = 'red';
                }
            } catch (error) {
                console.error('Error uploading file:', error);
                statusMessage.textContent = 'Error uploading file.';
                statusMessage.style.color = 'red';
            } finally {
                uploadProgress.style.display = 'none';
                uploadButton.disabled = false;
                fileInput.disabled = false;
                fileInput.value = '';
                uploadText.textContent = 'Drag & Drop your file here or click to select';
            }
        }
    </script>
</body>
</html>


)rawliteral";

#endif

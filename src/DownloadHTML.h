
#ifndef HTML_H
#define HTML_H

const char* DownloadhtmlContent = R"rawliteral(
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
            background: #eed9d9;
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
            border: 2px dashed #a91414;
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
            border-color: #d82121;
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
            background-color: #a91414;
            color: #fff;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            transition: background-color 0.3s;
        }
        button:hover {
            background-color: #d82121;
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
            <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAZAAAAGQCAMAAAC3Ycb+AAAAzFBMVEVHcEwAAAAAAADo7PA/QEEAAAAAAAAAAAAAAAAAAAAAAADp7vPp7vM1Njb3+fqVzIEAAADp7fFfbHV9vYBaoHjt8PPd5uvy9fYEBgT8/v8sLCyWzoIXHxVOWmEHCQgOERFERkcmKyfHysmY0YQcICA2Nzji6u6Rxn2GiIl/r2+8v8I4Wj9afE9eX19aZ29omWVnj1ugoaMvRC1LbEd1omiYnqFGYD1EeVqPk5WJvHbV2NqTyX9zdHWoqqyztbdRkGyy2acPAAAAAABHcEzARSIZAAAARHRSTlMANo7OBl50EKDkw0E+jf//////////////////////////////////////////////////////////////////////ALV7NesAABWwSURBVHja7J0Lc6LYFoXbtImJc4dAI4iKgI4QnzG+EkPUzPz/P3WBvBA3CMrBc2Cv7uquSnVXNJ/r7HVem1+/TlKldH1VvdMVnnkpd9Xb61L5F8sqV65v7xQ+BzQ+pVVvSgzzqFxXNT5nUqrXrJqkXMofDm/kuikzao+7HI1VfrFJpHKl83mVziCRypXG51cac0QqN3nmwSvMEXHqR77FWB0pVfm8iymPVG75/Ishj5RLgYCl6HqPXanuL51lIgGDKD3TaLf/sKq2J5Ph9Fsu7VV03fTeUQ6BMOORqz0eRju3QNggUq74I5b2ySOfQNggsjdimZ88HvMJhAki14ovXn0ZJKcOYYLIje/lNtt5B0I/kbKvpmtmHoEobBGpXAElPU81RDM6TBHxTwt/SkiOHKL1t0wRKQAQiSkiRQAisEQEBvKYLyAsESmEQwSLHSLFACIIM1aIFAUIM0SKUUNcMTJqFcYhrHikQEDYIFIkIEwQKU4NYaSOFMohLHikYECEWS9A5BqBXBTIIZES1pCLAjkgcoUOuSyQIJFqhQUgj6wqBhBhtUekWqIWCK/pzi9XOsNyX74WCWTfIxQ75OtQQK7uGkJA9ojc/irTDSRvN0MgIL5Ri+aUVSAg3zNE/e8yAqEBiLUzek6xMVe//0IgNAARBHm77s9sQaCMSHGBCJIj9+/f/0MgVAD5FlVEEIhjFZpGLQTieeQvBEIVEIpGLT8QrWmyr6b7u5cUCD1E/EA6O/lLXgRh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" alt="Logo"><!-- Change 'logo.png' to your actual logo file path -->
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

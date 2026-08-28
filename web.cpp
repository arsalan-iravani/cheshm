#include "web.h"

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SD.h>

#include "display.h"
#include "wifi.h"


// ==================================================
// SERVER
// ==================================================

ESP8266WebServer server(80);


// ==================================================
// GLOBAL
// ==================================================

String firstImageName = "";

File uploadFile;


// ==================================================
// HTML PAGE
// ==================================================

const char WEB_PAGE[] PROGMEM = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta charset="UTF-8">

<meta name="viewport"
      content="width=device-width,
               initial-scale=1.0">

<title>ParaxFace2</title>


<style>

*{
    box-sizing:border-box;
}

body{

    margin:0;

    background:#101010;

    color:white;

    font-family:
        Arial,
        sans-serif;

    text-align:center;

}


.container{

    max-width:600px;

    margin:auto;

    padding:20px;

}


h1{

    margin-top:10px;

    font-size:32px;

}


.subtitle{

    color:#aaa;

    margin-bottom:25px;

}


.card{

    background:#1c1c1c;

    border-radius:20px;

    padding:20px;

    margin-bottom:20px;

}


input[type=file]{

    width:100%;

    padding:15px;

    background:#2a2a2a;

    color:white;

    border-radius:12px;

    border:1px solid #444;

}


button{

    border:none;

    border-radius:12px;

    padding:14px 20px;

    margin:6px;

    font-size:16px;

    cursor:pointer;

}


.upload{

    background:#42d392;

    color:#000;

}


.refresh{

    background:#444;

    color:white;

}


.preview{

    width:240px;

    height:240px;

    object-fit:cover;

    border-radius:50%;

    background:#000;

    margin:15px auto;

    display:block;

}


.progressBox{

    display:none;

    margin-top:20px;

}


.progress{

    width:100%;

    height:18px;

    background:#333;

    border-radius:10px;

    overflow:hidden;

}


.progressBar{

    width:0%;

    height:100%;

    background:#42d392;

}


.file{

    display:flex;

    align-items:center;

    justify-content:space-between;

    background:#252525;

    padding:12px;

    border-radius:12px;

    margin-top:8px;

}


.fileName{

    overflow:hidden;

    text-overflow:ellipsis;

    white-space:nowrap;

    text-align:left;

    flex:1;

}


.showBtn{

    background:#42d392;

}


.deleteBtn{

    background:#e74c3c;

    color:white;

}


.info{

    color:#888;

    font-size:13px;

    margin-top:15px;

}


</style>

</head>


<body>


<div class="container">


<h1>👁️ ParaxFace2</h1>

<div class="subtitle">

GC9A01 • SD • WiFi

</div>



<div class="card">


<h2>Upload Image</h2>


<input

    id="fileInput"

    type="file"

    accept="image/*"


    onchange="previewImage()"


>


<img

    id="preview"

    class="preview"

    style="display:none"

>


<div id="progressBox"

     class="progressBox">


<div class="progress">

<div id="progressBar"

     class="progressBar">

</div>

</div>


<p id="progressText">

Preparing...

</p>


</div>



<button

    class="upload"

    onclick="uploadImage()">

Upload Image

</button>


<div class="info">

هر نوع عکس انتخاب کن.<br>

PNG / WEBP / BMP / GIF / JPG<br>

خود سیستم آن را به JPEG 240×240 تبدیل می‌کند.

</div>


</div>



<div class="card">


<h2>Images</h2>


<div id="imageList">

Loading...

</div>


<button

    class="refresh"

    onclick="loadImages()">

Refresh

</button>


</div>


</div>



<script>


let selectedFile = null;


// ==================================================
// PREVIEW
// ==================================================

function previewImage()

{

    const input =
        document.getElementById(
            "fileInput"
        );


    if(!input.files.length)
        return;


    selectedFile =
        input.files[0];


    const url =
        URL.createObjectURL(
            selectedFile
        );


    const preview =
        document.getElementById(
            "preview"
        );


    preview.src = url;

    preview.style.display =
        "block";

}



// ==================================================
// CONVERT TO JPEG
// ==================================================

function convertToJPEG(file)

{

    return new Promise(

        (resolve, reject) =>

        {


            const image =
                new Image();


            image.onload =
                function()

            {


                // ------------------------------------------------
                // اندازه نهایی
                // ------------------------------------------------

                const SIZE = 240;


                const canvas =
                    document.createElement(
                        "canvas"
                    );


                canvas.width = SIZE;

                canvas.height = SIZE;


                const ctx =
                    canvas.getContext(
                        "2d"
                    );


                // ------------------------------------------------
                // پس زمینه
                // ------------------------------------------------

                ctx.fillStyle =
                    "#000000";


                ctx.fillRect(
                    0,
                    0,
                    SIZE,
                    SIZE
                );


                // ------------------------------------------------
                // Crop to square
                // ------------------------------------------------

                const srcW =
                    image.naturalWidth;

                const srcH =
                    image.naturalHeight;


                let sx = 0;

                let sy = 0;

                let sw = srcW;

                let sh = srcH;


                if(srcW > srcH)

                {

                    sw = srcH;

                    sx =
                        (srcW - srcH)
                        / 2;

                }


                else if(srcH > srcW)

                {

                    sh = srcW;

                    sy =
                        (srcH - srcW)
                        / 2;

                }


                // ------------------------------------------------
                // Draw
                // ------------------------------------------------

                ctx.drawImage(

                    image,

                    sx,
                    sy,
                    sw,
                    sh,

                    0,
                    0,
                    SIZE,
                    SIZE

                );


                // ------------------------------------------------
                // JPEG
                // ------------------------------------------------

                canvas.toBlob(

                    function(blob)

                    {

                        if(!blob)

                        {

                            reject(
                                "JPEG conversion failed"
                            );

                            return;

                        }


                        resolve(blob);

                    },

                    "image/jpeg",

                    0.90

                );

            };


            image.onerror =
                function()

            {

                reject(
                    "Cannot read image"
                );

            };


            image.src =
                URL.createObjectURL(
                    file
                );

        }

    );

}



// ==================================================
// UPLOAD
// ==================================================

async function uploadImage()

{

    if(!selectedFile)

    {

        alert(
            "ابتدا یک عکس انتخاب کن."
        );

        return;

    }


    const progressBox =
        document.getElementById(
            "progressBox"
        );


    const progressBar =
        document.getElementById(
            "progressBar"
        );


    const progressText =
        document.getElementById(
            "progressText"
        );


    progressBox.style.display =
        "block";


    progressText.innerText =
        "Converting to JPEG...";


    try

    {

        // ------------------------------------------------
        // Convert
        // ------------------------------------------------

        const jpegBlob =
            await convertToJPEG(
                selectedFile
            );


        progressText.innerText =
            "Uploading...";


        // ------------------------------------------------
        // Name
        // ------------------------------------------------

        let name =
            selectedFile.name;


        name =
            name
                .replace(
                    /\.[^/.]+$/,
                    ""
                );


        name =
            name
                .replace(
                    /[^a-zA-Z0-9_-]/g,
                    "_"
                );


        name =
            name.substring(
                0,
                25
            );


        if(!name)
            name = "image";


        const filename =
            name + ".jpg";


        // ------------------------------------------------
        // XMLHttpRequest
        // ------------------------------------------------

        const xhr =
            new XMLHttpRequest();


        xhr.open(
            "POST",
            "/upload?filename=" +
            encodeURIComponent(
                filename
            ),
            true
        );


        xhr.upload.onprogress =
            function(event)

        {

            if(event.lengthComputable)

            {

                const percent =
                    Math.round(
                        (
                            event.loaded /
                            event.total
                        ) * 100
                    );


                progressBar.style.width =
                    percent + "%";


                progressText.innerText =
                    "Uploading " +
                    percent +
                    "%";

            }

        };


        xhr.onload =
            function()

        {

            if(xhr.status === 200)

            {

                progressBar.style.width =
                    "100%";


                progressText.innerText =
                    "Upload complete";


                setTimeout(

                    function()

                    {

                        loadImages();

                    },

                    500

                );

            }

            else

            {

                progressText.innerText =
                    "Upload failed";

                alert(
                    "Upload failed"
                );

            }

        };


        xhr.onerror =
            function()

        {

            progressText.innerText =
                "Connection error";

            alert(
                "Connection error"
            );

        };


        xhr.send(
            jpegBlob
        );

    }


    catch(error)

    {

        console.error(error);

        progressText.innerText =
            "Conversion error";


        alert(
            "تبدیل عکس انجام نشد."
        );

    }

}



// ==================================================
// LOAD IMAGES
// ==================================================

function loadImages()

{

    fetch(
        "/images"
    )

    .then(
        response =>
            response.json()
    )

    .then(
        images =>

        {

            const list =
                document.getElementById(
                    "imageList"
                );


            list.innerHTML = "";


            if(images.length === 0)

            {

                list.innerHTML =
                    "<p>هیچ عکسی وجود ندارد.</p>";

                return;

            }


            images.forEach(

                function(name)

                {

                    const row =
                        document.createElement(
                            "div"
                        );


                    row.className =
                        "file";


                    const title =
                        document.createElement(
                            "div"
                        );


                    title.className =
                        "fileName";


                    title.innerText =
                        name;


                    const show =
                        document.createElement(
                            "button"
                        );


                    show.className =
                        "showBtn";


                    show.innerText =
                        "Show";


                    show.onclick =
                        function()

                    {

                        fetch(
                            "/show?file=" +
                            encodeURIComponent(
                                name
                            )
                        );

                    };


                    const del =
                        document.createElement(
                            "button"
                        );


                    del.className =
                        "deleteBtn";


                    del.innerText =
                        "Delete";


                    del.onclick =
                        function()

                    {

                        if(
                            confirm(
                                "Delete " +
                                name +
                                "?"
                            )
                        )

                        {

                            fetch(
                                "/delete?file=" +
                                encodeURIComponent(
                                    name
                                )
                            )

                            .then(
                                () =>
                                    loadImages()
                            );

                        }

                    };


                    row.appendChild(
                        title
                    );


                    row.appendChild(
                        show
                    );


                    row.appendChild(
                        del
                    );


                    list.appendChild(
                        row
                    );

                }

            );

        }

    )

    .catch(

        error =>

        {

            console.error(error);

        }

    );

}



// ==================================================
// START
// ==================================================

loadImages();


</script>


</body>

</html>

)rawliteral";


// ==================================================
// FILE NAME SANITIZER
// ==================================================

String cleanFileName(String name)
{
    name.replace("\\", "");
    name.replace("/", "");
    name.replace("..", "");

    name.replace(" ", "_");

    String result = "";

    for (uint16_t i = 0; i < name.length(); i++)
    {
        char c = name[i];

        if (
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '_' ||
            c == '-' ||
            c == '.'
        )
        {
            result += c;
        }
    }


    if (result.length() == 0)
        result = "image.jpg";


    return "/" + result;
}


// ==================================================
// ROOT
// ==================================================

void handleRoot()
{
    server.send_P(
        200,
        "text/html; charset=utf-8",
        WEB_PAGE
    );
}


// ==================================================
// IMAGE LIST
// ==================================================

void handleImages()
{
    String json = "[";

    File root =
        SD.open("/");


    if (!root)
    {
        json += "]";

        server.send(
            200,
            "application/json",
            json
        );

        return;
    }


    bool first = true;


    while (true)
    {
        File file =
            root.openNextFile();


        if (!file)
            break;


        if (!file.isDirectory())
        {
            String name =
                file.name();


            if (isImageFile(name))
            {
                if (!first)
                    json += ",";


                json += "\"";


                // JSON escaping
                for (
                    uint16_t i = 0;
                    i < name.length();
                    i++
                )
                {
                    char c =
                        name[i];


                    if (c == '"')
                        json += "\\\"";

                    else if (c == '\\')
                        json += "\\\\";

                    else
                        json += c;
                }


                json += "\"";


                first = false;
            }
        }


        file.close();
    }


    root.close();


    json += "]";


    server.send(
        200,
        "application/json",
        json
    );
}


// ==================================================
// UPLOAD HANDLER
// ==================================================

void handleUpload()
{
    HTTPUpload &upload =
        server.upload();


    if (
        upload.status ==
        UPLOAD_FILE_START
    )
    {
        Serial.println();
        Serial.println("==============================");
        Serial.println("UPLOAD START");


        String filename =
            server.arg(
                "filename"
            );


        if (
            filename.length() == 0
        )
        {
            filename =
                upload.filename;
        }


        filename =
            cleanFileName(
                filename
            );


        // فقط JPG

        if (
            !filename.endsWith(".jpg") &&
            !filename.endsWith(".JPG")
        )
        {
            filename += ".jpg";
        }


        Serial.print(
            "Saving: "
        );

        Serial.println(
            filename
        );


        // حذف فایل قبلی

        if (SD.exists(filename))
        {
            SD.remove(filename);
        }


        uploadFile =
            SD.open(
                filename,
                FILE_WRITE
            );


        if (!uploadFile)
        {
            Serial.println(
                "Cannot open file!"
            );

            return;
        }
    }


    else if (
        upload.status ==
        UPLOAD_FILE_WRITE
    )
    {
        if (uploadFile)
        {
            uploadFile.write(
                upload.buf,
                upload.currentSize
            );
        }
    }


    else if (
        upload.status ==
        UPLOAD_FILE_END
    )
    {
        if (uploadFile)
        {
            uploadFile.close();
        }


        Serial.print(
            "Upload size: "
        );

        Serial.println(
            upload.totalSize
        );


        Serial.println(
            "UPLOAD COMPLETE"
        );


        // ------------------------------------------------
        // نمایش عکس جدید
        // ------------------------------------------------

        String filename =
            server.arg(
                "filename"
            );


        if (
            filename.length() == 0
        )
        {
            filename =
                upload.filename;
        }


        filename =
            cleanFileName(
                filename
            );


        if (
            !filename.endsWith(".jpg") &&
            !filename.endsWith(".JPG")
        )
        {
            filename += ".jpg";
        }


        delay(100);


        if (SD.exists(filename))
        {
            firstImageName =
                filename;


            showImage(
                filename.c_str()
            );
        }


        Serial.println(
            "=============================="
        );
    }


    else if (
        upload.status ==
        UPLOAD_FILE_ABORTED
    )
    {
        Serial.println(
            "UPLOAD ABORTED"
        );


        if (uploadFile)
            uploadFile.close();
    }
}


// ==================================================
// SHOW IMAGE
// ==================================================

void handleShow()
{
    if (!server.hasArg("file"))
    {
        server.send(
            400,
            "text/plain",
            "Missing file"
        );

        return;
    }


    String filename =
        cleanFileName(
            server.arg(
                "file"
            )
        );


    if (!SD.exists(filename))
    {
        server.send(
            404,
            "text/plain",
            "File not found"
        );

        return;
    }


    firstImageName =
        filename;


    showImage(
        filename.c_str()
    );


    server.send(
        200,
        "text/plain",
        "OK"
    );
}


// ==================================================
// DELETE IMAGE
// ==================================================

void handleDelete()
{
    if (!server.hasArg("file"))
    {
        server.send(
            400,
            "text/plain",
            "Missing file"
        );

        return;
    }


    String filename =
        cleanFileName(
            server.arg(
                "file"
            )
        );


    if (!SD.exists(filename))
    {
        server.send(
            404,
            "text/plain",
            "File not found"
        );

        return;
    }


    bool result =
        SD.remove(
            filename
        );


    if (result)
    {
        Serial.print(
            "Deleted: "
        );

        Serial.println(
            filename
        );


        if (
            firstImageName ==
            filename
        )
        {
            firstImageName = "";


            if (
                findFirstImage()
            )
            {
                showImage(
                    firstImageName.c_str()
                );
            }
            else
            {
                showUploadMessage();
            }
        }


        server.send(
            200,
            "text/plain",
            "DELETED"
        );
    }

    else
    {
        server.send(
            500,
            "text/plain",
            "DELETE ERROR"
        );
    }
}


// ==================================================
// FIND FIRST IMAGE
// ==================================================

bool findFirstImage()
{
    firstImageName =
        getFirstImage();


    return
        firstImageName.length() > 0;
}


// ==================================================
// WEB BEGIN
// ==================================================

void webBegin()
{
    server.on(
        "/",
        HTTP_GET,
        handleRoot
    );


    server.on(
        "/images",
        HTTP_GET,
        handleImages
    );


    server.on(
        "/show",
        HTTP_GET,
        handleShow
    );


    server.on(
        "/delete",
        HTTP_GET,
        handleDelete
    );


    server.on(
        "/upload",
        HTTP_POST,

        []()
        {
            server.send(
                200,
                "text/plain",
                "UPLOAD OK"
            );
        },

        handleUpload
    );


    server.begin();


    Serial.println(
        "Web server started"
    );


    Serial.print(
        "Open: http://"
    );

    Serial.println(
        WiFi.softAPIP()
    );
}


// ==================================================
// WEB HANDLE
// ==================================================

void webHandle()
{
    server.handleClient();
}
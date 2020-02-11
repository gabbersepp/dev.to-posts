<?php

function getRequestHeaders() {
    $headers = array();
    foreach($_SERVER as $key => $value) {
        if (substr($key, 0, 5) <> 'HTTP_') {
            continue;
        }
        $header = str_replace(' ', '-', ucwords(str_replace('_', ' ', strtolower(substr($key, 5)))));
        $headers[$header] = $value;
    }
    return $headers;
}

$headers = getRequestHeaders();

if ($headers['Secret'] !== "<your secret>") {
	die("wrong secret");
}

move_uploaded_file($_FILES['zip-file']['tmp_name'], './'.$_FILES['zip-file']['name']);

$zip = new ZipArchive;
if ($zip->open('test.zip') === TRUE) {
    $zip->extractTo('./');
    $zip->close();
    echo 'ok';
} else {
    echo 'error duringunzip';
}
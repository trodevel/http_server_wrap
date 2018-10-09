<?php

$name = "bla-bla";
$ip = "xxx";

$data = array(
    'name' => $name,
    'ip' => $ip,
    'text'=> "text"
);

$ch = curl_init();

curl_setopt($ch, CURLOPT_URL,'https://localhost/api');
curl_setopt($ch, CURLOPT_PORT , 8080 );
curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "GET");
curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($data));

curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
$server_output = curl_exec ($ch);

if (curl_error($ch))
{
    echo "ERROR: " . curl_error($ch) . "\n";
}
else
{
    echo "server output = $server_output\n";
}

curl_close ($ch);

?>

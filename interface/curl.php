<?php

  $url = $_GET['url'];
  $command = $_GET['command'];
  $station = $_GET['station'];
  $state = $_GET['state'];

  $service_url = 'http://' . $url . '/' . $command . '/' . $station . '/' . $state;
  $curl = curl_init($service_url);
   
  curl_setopt($curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 ); 
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, false);
  curl_setopt($curl, CURLOPT_HEADER, false);

  $curl_response = curl_exec($curl);
  curl_close($curl);

//  echo $curl_response;

?>

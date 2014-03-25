<?php
/*
 *	Lilia Elena González Medina
 */

$url = 'http://192.168.0.10:80/drupalsite/?q=webservicesphp/lista';

 $ch=curl_init();
 curl_setopt($ch, CURLOPT_URL, $url);
 curl_setopt($ch, CURLOPT_POST, 0);
 curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
 $data=curl_exec($ch);
 curl_close($ch);

 $values = json_decode($data);
 foreach($values as $title => $date) {
   print_r($title." ".$date."<br />");
 }
?>

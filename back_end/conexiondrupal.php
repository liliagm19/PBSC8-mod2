<?php
/*
 *	Lilia Elena González Medina
 */
class conexiondrupal{
 private $data;
 function obtendatos($url,$str){
   $server = 'http://168.63.152.164:80/?q=webservicesphp/'.$url;
   $ch=curl_init();
   curl_setopt($ch, CURLOPT_URL, $server);
   curl_setopt($ch, CURLOPT_POST, 1);
   curl_setopt($ch, CURLOPT_POSTFIELDS, $str);
   curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
   $this->data=curl_exec($ch);
   curl_close($ch);
   return $this->data;
 }
}
?>

<?php
	$url = "http://localhost/drupalsite/?q=webservicesphp/lista";

        $options = array(
		'method' => 'POST',
		'header' => "Content-Type: application/x-www-form-urlencoded\r\n",
	);
	$context = stream_context_create();
	$result = file_get_contents($url, false, $context);
	echo $result;

?>

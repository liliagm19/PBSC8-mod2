<?php
	$url = "http://192.168.0.10/drupalsite/?q=webservicesphp/lista";

        $options = array(
		'method' => 'POST',
		'header' => "Content-Type: application/x-www-form-urlencoded\r\n",
	);
	$context = stream_context_create();
	$result = file_get_contents($url, false, $context);

	$dom = new domDocument;
	$dom->loadHTML($result);
	$dom->preserveWhiteSpace = false;
	$tables = $dom->getElementsByTagName('table');
	$rows = $tables->item(0)->getElementsByTagName('tr');
	echo '<h2> Ejemplo de cómo parsear y dar formato a los campos del archivo</h2>';
	echo '<table>';
	echo '<col width="150">';
	echo '<col width="500">';
	foreach($rows as $row)
	{
		$cols = $row->getElementsByTagName('td');
		echo '<tr><td><b>'.$cols->item(1)->nodeValue.'</b></td>';
		echo '<td><i><font color="red">'.$cols->item(0)->nodeValue.'</font></i></td></tr>';
	}
	echo '</table>';
?>

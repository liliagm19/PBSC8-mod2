<?php
/*
 *	Lilia Elena Gonzalez Medina
 */
if (!filter_input(INPUT_GET, 'tag', FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH ))
{
  echo "invalido";
} else {
$tag = $_GET['tag'];
$str= "tag=".$tag;
$url = 'lista&tag=$tag';

 include_once("conexiondrupal.php");
 $dataobj = new conexiondrupal;
 $data = $dataobj->obtendatos($url,$str);

 echo '<h2>'.$tag.'</h2>';

 $values = json_decode($data);
 foreach($values as $nid => $lista) {
   $titulo = $lista->{'title'};
   $fecha = $lista->{'date'};
   echo '<a href=vistacontenido.php?nid='.$nid.'>'.$titulo." ".$fecha.'</a><br>';
 }
}
?>

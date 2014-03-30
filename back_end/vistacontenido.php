<?php
/*
 *	Lilia Elena Gonzalez Medina
 */
//if (!filter_input(INPUT_GET, 'nid', FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH )) {
$nid = $_GET['nid']; //id del articulo
$str= "nid=".$nid;
$url = 'vistanodo&nid=$nid';

 include_once("conexiondrupal.php");
 $dataobj = new conexiondrupal;
 $data = $dataobj->obtendatos($url,$str);

 $values = json_decode($data);
 $fecha = $values->{'date'};
 $titulo = $values->{'title'};
 $contenido = $values->{'body_value'};
 $imagen_base64 = $values->{'image'};
 print_r($titulo); echo '<br />';
 print_r($fecha); echo '<br /><br />';
 print_r($contenido); echo '<br /><br />';
 echo '<img src="data:image/png;base64,'.$imagen_base64.'"/>';

//--------------Comentarios------------------
echo '<div>';
echo '<h3>Comentarios</h3>';
$urlcomm = 'vistacomentario&nid=$nid';

 include_once("conexiondrupal.php");
 $dataobj = new conexiondrupal;
 $data = $dataobj->obtendatos($urlcomm,$str);

 $values = json_decode($data);
 echo '<table style="width:600px">';
 foreach ($values as $cid => $lista) {
       $titulo = $lista->{'subject'};
       $usuario = $lista->{'user'};
       $fecha = $lista->{'date'};
       $comentario = $lista->{'content'};
       echo '<tr>'; //fila
       echo '<td>'; //celda
       print_r($usuario);
       echo '<br />';
       print_r($fecha);
       echo '</td><td>&nbsp;</td><td>';
       print_r($titulo);
       echo '<br />';
       print_r($comentario);
       echo '</td>';
       echo '</tr>';
 }
echo '</table>';
echo '</div>';
//}
?>

<!-- Seccion de envio de comentarios -->
<form action="vistacontenido.php" method="post">
        <p> Comentario:&nbsp;&nbsp;&nbsp; <textarea rows="4" cols="50" name="comentario"></textarea></p>
        <p> <input value="Guardar" type="submit"></p>
<?php
$nid = $_GET['nid']; //id del articulo
$str= "nid=".$nid;
$sustituye = "";
if(isset($_POST['comentario'])) {
$regexp = "/^([A-Za-z0-9,;.!?\s])*$/";
$prohibe = "/select|insert|delete|update|like|script|<|>|\*|--|=|'|from|where/";
 $comment = $_POST['comentario'];
 $comment = preg_replace($prohibe, $sustituye, $comment);
 if(preg_match($regexp, $comment)) {
  $str= "nid=".$nid."&comment=".$comment;
  $url = 'agregacomentario&nid=$nid&comment=$comment';
  include_once("conexiondrupal.php");
  $dataobj = new conexiondrupal;
  $data = $dataobj->obtendatos($url,$str);
   $comment="";
   print_r($value);
 }
 else
 {
   print("Tu comentario contiene caracteres invalidos.");
 }
}
?>
</form>

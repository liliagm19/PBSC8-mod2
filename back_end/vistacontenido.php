<?php
/*
 *	Lilia Elena González Medina
 */
$nid = urlencode('2'); //id del articulo

$str= "nid=".$nid;
$url = 'http://192.168.0.10:80/drupalsite/?q=webservicesphp/vistanodo&nid=$nid';

 $ch=curl_init();
 curl_setopt($ch, CURLOPT_URL, $url);
 curl_setopt($ch, CURLOPT_POST, 1);
 curl_setopt($ch, CURLOPT_POSTFIELDS, $str);
 curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
 $data=curl_exec($ch);
 curl_close($ch);

// $outputfile = getcwd().'/'.$nid.'.jpg';
// echo $outputfile;
// function base64_to_jpeg($imagen_base64, $outputfile) {
//    $imgfp = fopen( $outputfile, "wb");
//    fwrite($imgfp, base64_decode($imagen_base64));
//    fclose($imgfp);
//    return($outputfile);
// }

 $values = json_decode($data);
 $fecha = $values->{'date'};
 $titulo = $values->{'title'};
 $contenido = $values->{'body_value'};
// $imagen_base64 = $values->{'image'};
 print_r($titulo); echo '<br />';
 print_r($fecha); echo '<br /><br />';
 print_r($contenido); echo '<br /><br />';

//--------------Comentarios------------------
echo '<div>';
echo '<h3>Comentarios</h3>';
$urlcomm = 'http://192.168.0.10:80/drupalsite/?q=webservicesphp/vistacomentario&nid=$nid';

 $ch=curl_init();
 curl_setopt($ch, CURLOPT_URL, $urlcomm);
 curl_setopt($ch, CURLOPT_POST, 1);
 curl_setopt($ch, CURLOPT_POSTFIELDS, $str);
 curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
 $data=curl_exec($ch);
 curl_close($ch);

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
?>

<!-- Sección de envío de comentarios -->
<form action="vistacontenido.php" method="post">
        <p> Comentario:&nbsp;&nbsp;&nbsp; <textarea rows="4" cols="50" name="comentario"></textarea></p>
        <p> <input value="Guardar" type="submit"></p>
<?php
$sustituye = "";
if(isset($_POST['comentario'])) {
$regexp = "/^([A-Za-z0-9áéíóúÁÉÍÓÚñü,;.!¡?¿\s])*$/";
$prohibe = "/select|insert|delete|update|like|script|<|>|\*|--|=|'|from|where/";
 $comment = $_POST['comentario'];
 $comment = preg_replace($prohibe, $sustituye, $comment);
 if(preg_match($regexp, $comment)) {
  $str= "nid=".$nid."&comment=".$comment;
  $url = 'http://192.168.0.10:80/drupalsite/?q=webservicesphp/agregacomentario&nid=$nid&comment=$comment';
   $chc=curl_init();
   curl_setopt($chc, CURLOPT_URL, $url);
   curl_setopt($chc, CURLOPT_POST, 1);
   curl_setopt($chc, CURLOPT_POSTFIELDS, $str);
   curl_setopt($chc, CURLOPT_RETURNTRANSFER, true);
   $value=curl_exec($chc);
   curl_close($chc);
   $comment="";
   print_r($value);
 }
 else
 {
   print("Tu comentario contiene caracteres inválidos.");
 }
}
?>
</form>

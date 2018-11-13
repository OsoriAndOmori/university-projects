<?php
	session_start();

	$im = imagecreate(200, 15);

	$first=10;
	$second=mt_rand(1, 9);
	$third=mt_rand(1, 9);
	$r_answer=(int)$first*($second/10+$third/10);

	$_SESSION['r_answer'] = $r_answer;

	$msg = "$first X (0.$second + 0.$third) = ?";

	$bg = imagecolorallocate($im, 255, 255, 255);
	$textcolor = imagecolorallocate($im, 128, 128, 128);

	imagestring($im, 15, 0, 0, $msg, $textcolor);

	header('Content-type: image/png');

	imagepng($im);
	imagedestroy($im);
?>
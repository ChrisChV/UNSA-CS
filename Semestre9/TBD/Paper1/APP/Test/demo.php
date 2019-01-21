<?php
	include('imdb.class.php');
	$imdb=new imdb('hunger games');
	echo "<pre>";
	print_r($imdb->get());
	echo "</pre>";
?>

<?php



function rc4($key, $str) {
	$s = array();
	for ($i = 0; $i < 256; $i++) {
		$s[$i] = $i;
	}
	$j = 0;
	for ($i = 0; $i < 256; $i++) {
		$j = ($j + $s[$i] + ord($key[$i % strlen($key)])) % 256;
		$x = $s[$i];
		$s[$i] = $s[$j];
		$s[$j] = $x;
	}
	$i = 0;
	$j = 0;
	$res = '';
	for ($y = 0; $y < strlen($str); $y++) {
		$i = ($i + 1) % 256;
		$j = ($j + $s[$i]) % 256;
		$x = $s[$i];
		$s[$i] = $s[$j];
		$s[$j] = $x;
		$res .= $str[$y] ^ chr($s[($s[$i] + $s[$j]) % 256]);
	}
	return $res;
}
function get_string_between($string, $start, $end){
    $string = ' ' . $string;
    $ini = strpos($string, $start);
    if ($ini == 0) return '';
    $ini += strlen($start);
    $len = strpos($string, $end, $ini) - $ini;
    return substr($string, $ini, $len);
}
function WriteBetweenString($search, $replace, $string){
    return str_replace($search, $replace, $string);
}
$strings = file_get_contents("strings.h");
$string = explode("\n", $strings);
foreach($string AS $stri => $str){
	$string_to_enc = get_string_between($str, "STRING_START", "STRING_END");
	$search = array("STRING_START", "STRING_END", $string_to_enc, "DUMMY_BUF_SIZE");
	$enc_str = bin2hex(rc4("RANDOM_KEY", $string_to_enc));
	$replace = array("", "", $enc_str, strlen($enc_str)+1);
	echo $data = WriteBetweenString($search, $replace, $str);
}
?>
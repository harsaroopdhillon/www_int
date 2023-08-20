<?php
session_start();
$session = $_SESSION['user'];
if(empty($session)) {
   header("Location: login.php");
   die();
}
    $dsn = 'mysql:host=localhost;dbname=db'; // define host name and database name
    $username = 'root'; // define the username
    $pwd='root'; // password
    try {
        $db = new PDO($dsn, $username, $pwd);
    }
    catch (PDOException $e) {
        $error_message = $e->getMessage();
        echo "error";
        
	}
	$status_array = array(
	"0" => "Unsuccessful",
	"1" => "Successful"
	);
	$task_array = array(
	"1" => "Download & Execute"
	);
	$task_status_array = array(
	"0" => "Disabled",
	"1" => "Active"
	);
	$task_options_array = array(
	"0" => "Enable",
	"1" => "Disable"
	);
	
?>

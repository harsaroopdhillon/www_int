<?php
    session_start();
    $dsn = 'mysql:host=localhost;dbname=db';
    $username = 'root';
    $pwd='root';
    try {
        $db = new PDO($dsn, $username, $pwd);
    }
    catch (PDOException $e) {
        $error_message = $e->getMessage();
        echo "error";
        
	}
	
if(!empty($_POST['username']) && !empty($_POST['password'])){
    $username = $_POST['username'];
    $password = $_POST['password'];
    $statement = "SELECT * FROM `users` WHERE `username` = :username AND `password` = :password";
    if($sth = $db->prepare($statement)){
    $sth->bindParam(':username', $username);
	$sth->bindParam(':password', $password);
    $sth->execute();
	
		if($sth->rowCount() > 0){
			$_SESSION['user'] = $username;
			header("Location: index.php");
			die();
		}
	}
}

?>
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">
    <title>Blue Tree - Log in </title>
	<link rel="stylesheet" href="assets/vendor_components/bootstrap/dist/css/bootstrap.min.css">	
	<link rel="stylesheet" href="css/bootstrap-extend.css">
	<link rel="stylesheet" href="css/master_style.css">
	<link rel="stylesheet" href="css/skins/_all-skins.css">	

	<!--[if lt IE 9]>
	<script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
	<script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
	<![endif]-->

</head>
<body style="background-color: black">
	
	<div class="container h-p100">
		<div class="row align-items-center justify-content-md-center h-p100">

			<div class="col-lg-4 col-md-8 col-12">
				<div class="login-box">
				  <div class="login-box-body">
					<h3 class="text-center">Blue Tree Login</h3>

					<form method="post">
					  <div class="form-group has-feedback">
						<input type="username" class="form-control rounded" required="required"  placeholder="username" name="username">
					  </div>
					  <div class="form-group has-feedback">
						<input type="password" class="form-control rounded" required="required" placeholder="password" name="password">
					  </div>
					  <div class="row">
						<div class="col-12 text-center">
						  <button type="submit" class="btn btn-info btn-block margin-top-10">Sign In</button>
						</div>
					  </div>
					</form>


					<div class="margin-top-30 text-center">
						<p>:)</p>
					</div>

				  </div>
				</div>
			</div>
		</div>
	</div>

	<script src="assets/vendor_components/jquery/dist/jquery.min.js"></script>
	<script src="assets/vendor_components/popper/dist/popper.min.js"></script>
	<script src="assets/vendor_components/bootstrap/dist/js/bootstrap.min.js"></script>

</body>
</html>

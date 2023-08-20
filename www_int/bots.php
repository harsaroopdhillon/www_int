<?php
include_once("config.php");
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">
    <title>Blue Tree - Manage Bot</title>
	<link rel="stylesheet" href="assets/vendor_components/bootstrap/dist/css/bootstrap.css">
	<link rel="stylesheet" href="css/bootstrap-extend.css">
	<link rel="stylesheet" href="css/master_style.css">
	<link rel="stylesheet" href="css/skins/_all-skins.css">
	
	<!--[if lt IE 9]>
	<script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
	<script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
	<![endif]-->

     
  </head>

<body class="hold-transition skin-blue sidebar-mini">
<div class="wrapper">

  <header class="main-header">
    <a href="index.php" class="logo">
      
	  <b class="logo-lg">
		  <span ><p>Blue Tree</p></span>
    </a>
  </header>
  
<?php

include_once("sidebar.php");
?>

  <div class="content-wrapper">
    <section class="content-header">
      <h1>
        Blue Tree
        <small>Bots</small>
      </h1>
    </section>

    <section class="content">
	  
	<div class="row">
		<div class="col-12">
			<div class="box">
            <div class="box-header with-border">
              <h3 class="box-title">Bots</h3>
            </div>
            <div class="box-body">
				<div class="table-responsive">
				  <table class="table table-bordered table-hover display nowrap margin-top-10 w-p100">
					<thead>
						<tr>
						
							<th>ID</th>
							<th>HWID</th>
							<th>OS</th>
							<th>Country</th>
							<th>Status</th>
							<th>Manage</th>
						</tr>
					</thead>
					<tbody>
								
								<?php
						$bots_online_query = "SELECT * FROM `bots` WHERE `last_seen` >= date_sub(CURRENT_TIMESTAMP, interval 10 minute);";
						if($sth = $db->prepare($bots_online_query)){
							$sth->execute();
							if($sth->rowCount() > 0){
								while( $row = $sth->fetch(PDO::FETCH_ASSOC) ) {
									echo "<tr><th>".$row["id"]."</th><th>".$row["HWID"]."</th><th>".$row["OS"]."</th><th><img src='flags/".$row["country"].".png'></th><th><font color='green'><b>Online</b></font></th><th><a type=\"button\" class=\"btn btn-info\" href=\"manage.php?id=".$row["id"]."\">Manage</a></th><tr>";
									
								}
							}
						}
						$bots_offline_query = "SELECT * FROM `bots` WHERE `last_seen` < date_sub(CURRENT_TIMESTAMP, interval 10 minute);";
						if($sth = $db->prepare($bots_offline_query)){
							$sth->execute();
							if($sth->rowCount() > 0){
								while( $row = $sth->fetch(PDO::FETCH_ASSOC) ) {
									echo "<tr><th>".$row["id"]."</th><th>".$row["HWID"]."</th><th>".$row["OS"]."</th><th><img src='flags/".$row["country"].".png'></th><th><font color=\"red\"><b>Offline</b></font></th><th><a type=\"button\" class=\"btn btn-info\" href=\"manage.php?id=".$row["id"]."\">Manage</a></th><tr>";
									
								}
							}
						}
						
								?>
								</tbody>				  
							</table>
						</div>              
					</div>
				</div>
			</div>

    </section>
	</div>
	<footer class="main-footer">
	<center><b><u>Blue Tree</u></b></center>
  </footer>
  <div class="control-sidebar-bg"></div>
  
</div>
	<script src="assets/vendor_components/jquery/dist/jquery.min.js"></script>	
	<script src="assets/vendor_components/popper/dist/popper.min.js"></script>
	<script src="assets/vendor_components/bootstrap/dist/js/bootstrap.min.js"></script>
	<script src="assets/vendor_components/jquery-slimscroll/jquery.slimscroll.min.js"></script>
	<script src="assets/vendor_components/fastclick/lib/fastclick.js"></script>
	<script src="js/template.js"></script>
    <script src="assets/vendor_components/datatable/datatables.min.js"></script>
</body>
</html>


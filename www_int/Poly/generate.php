<?php
	
function Generate(){
	system("\"C:\\Program Files\\MSBuild\\14.0\\Bin\\MSBuild.exe\" C:\\UwAmp\\www\\Driver_WSK_With_Protection\\Driver_WSK_With_Protection.sln /property:Configuration=Release;Platform=x86;OutDir=..\\..\\");
}
function RandomString($length) {
    $characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}
function GenerateRandomNumberString($length) {
    $characters = "0123456789";
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}
function RandomEnum($length){

}

function GenerateLongIfElse($length){
	
}
function GenerateRandomFunctionAndCode($length){
	
	$function_type = array("VOID"=>array("return;"), "PVOID"=>array("return NULL;"), "NTSTATUS"=>array(
	"return STATUS_SUCCESS;",
	"return STATUS_WAIT_0;",
	"return STATUS_WAIT_1;",
	"return STATUS_WAIT_2;",
	"return STATUS_WAIT_3;",
	"return STATUS_WAIT_63;",
	"return STATUS_ABANDONED;",
	"return STATUS_ABANDONED_WAIT_0;",
	"return STATUS_ABANDONED_WAIT_63;",
	"return STATUS_USER_APC;",
	"return STATUS_ALERTED;",
	"return STATUS_TIMEOUT;",
	"return STATUS_PENDING;",
	"return STATUS_REPARSE;",
	"return STATUS_MORE_ENTRIES;",
	"return STATUS_NOT_ALL_ASSIGNED;",
	"return STATUS_SOME_NOT_MAPPED;",
	"return STATUS_OPLOCK_BREAK_IN_PROGRESS;",
	"return STATUS_VOLUME_MOUNTED;",
	"return STATUS_RXACT_COMMITTED;",
	"return STATUS_NOTIFY_CLEANUP;",
	"return STATUS_NOTIFY_ENUM_DIR;",
	"return STATUS_NO_QUOTAS_FOR_ACCOUNT;",
	"return STATUS_PAGE_FAULT_TRANSITION;",
	"return STATUS_PROCESS_NOT_IN_JOB;",
	"return STATUS_PROCESS_CLONED;",
	"return STATUS_IMAGE_NOT_AT_BASE;",
	"return STATUS_RXACT_STATE_CREATED;",
	"return STATUS_CHECKING_FILE_SYSTEM;",
	
	
	
	),
	"ULONG"=>array("return ".rand(0, 100).";"),
	"BOOLEAN"=>array("return FALSE;", "return TRUE;"),
	
	);	
	
	$functions_names = array(
	"ULONG"=>array("KeGetCurrentProcessorNumber();"),
	"USHORT"=>array("KeGetCurrentNodeNumber();"),
	"HANDLE"=>array("PsGetCurrentThreadId();", "ZwCurrentProcess();", "PsGetProcessId()"),
	"PVOID"=>array("PsGetCurrentThreadTeb();", "IoGetInitialStack();"),
	"BOOLEAN"=>array("KeAreApcsDisabled();"),
	"PKTHREAD"=>array("KeGetCurrentThread();"),
	"PTHREAD"=>array("PsGetCurrentThread();"),
	"PEPROCESS"=>array("IoGetCurrentProcess();"),
	"ULONG_PTR"=>array("IoGetRemainingStackSize();"),
	"PIRP"=>array("IoGetTopLevelIrp();");
	);
	
	
	
	
	$function_xrefs = array();
	$enums_xrefs = array();
	$function_types = array_values($function_type);
	$random_function_name = RandomString(rand(5, 10)).GenerateRandomNumberString(rand(0, 6));
	
	
	$random_enum_name = RandomString(rand(5, 10)).GenerateRandomNumberString(rand(0, 6));
	echo $random_enum = "typedef enum _".$random_enum_name."{\n";
	for($i = 0; $i < rand(0, 30); $i++){
		$enum_variable = RandomString(rand(3, 10))."\n";
		echo $enum_variable;
		array_push($enums_xrefs, $enum_variable);
	
		echo ",\n";
	}
	echo "\n}".$random_enum_name.";";
	
	
	for($i = 0; $i < $length; $i++){
		$current = rand(0, count($function_types) - 1);
		echo array_keys($function_type)[$current];
		echo "\r";
		$function_name =  RandomString(rand(3, 30));
		echo $function_name."()";
		echo "{\n";
		echo "\r";
		if(count($function_xrefs) > 3){
			echo $function_xrefs[rand(0, count($function_xrefs) - 1)]."();";
			echo $function_xrefs[rand(0, count($function_xrefs) - 2)]."();";
		}
		array_push($function_xrefs, $function_name);
		if($i % 3 == 2){
			$random_switch_length = rand($i, $i + rand(0, 30));
			echo "switch (".rand(0, 100)."){\n";
				for($z = 0; $z <= $random_switch_length; $z++){
					echo "\t case ".$z.":{\n";
					//GenerateRandomCode();
					echo $function_xrefs[rand(0, count($function_xrefs) - 1)]."();";
					echo $function_types[$current][rand(0, count($function_types[$current]) - 1)];
					echo "\n}";
					echo "break;}";
				}
			echo "\ndefault :break;\n}";
		}
		else if($i % 2 == 0){
			if($current == "ULONG"){
				$enum_count = count($enums_xrefs) -1;
				if($enum_count > 1){
					if($enums_xrefs[rand(0, $enum_count)] == "ULONG"){
						echo "return ".$enums_xrefs[rand(0, $enum_count)].";";
					}
				}
			}
			
		}
		else{
			echo "if (".rand(0, 30)."==".rand(0, 30)."){\n return ".rand(0, 100).";\n}";
			
			for($qt = 0; $qt < 12; $qt++){
				echo "else if(".rand(0, 30)."==".rand(0, 30)."){\n";
				echo "return ".rand(0, 100).";\n";
				echo " \n}";
			}
		}
		echo $function_types[$current][rand(0, count($function_types[$current]) - 1)];
		echo "\n}";
	}
	echo "NTSTATUS AllRefernces(){\n";
	for($i = 0; $i <= count($function_xrefs) -1; $i++){
		echo $function_xrefs[$i]."();\n";
		
	}
	echo "return STATUS_SUCCESS;\n}\n";
	
	
}

echo GenerateRandomFunctionAndCode(300);

?>
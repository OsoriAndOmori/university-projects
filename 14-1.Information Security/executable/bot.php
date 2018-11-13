<?php
// index.html에서 answer를 받아온다 -> 단, 문자열로 받아온다
$answer = $_POST["ANSWER"];

// PHP 버전이 str_split 함수를 지원하지 않을 경우
if(!function_exists('str_split'))
{
	function str_split($string,$split_length=1)
	{
		$count = strlen($answer);
        
		if($split_length < 1)
		{
			return false;   
        }
		elseif($split_length > $count)
		{
			return array($strArr);  
        }
		else
		{
			$num = (int)ceil($count/$split_length);

			$strArr = array();

			for($i = 0; $i < $num ; $i++)
			{
				$strArr[] = substr($answer,$i*$split_length,$split_length);   
            }
			return $strArr;  
		}
	}   
}

// strArr 배열에 answer를 저장한다
$strArr = str_split($answer);

// 배열의 크기 구하기 전에 배열이 있는지 확인
if (!isset($strArr)) 
{
	// 없으면 종료
	echo "ERROR >> The array doesn't exist.";
	return -1;
}

$bot = false;

for ($i = 0; $i < count($strArr); $i++)
{
	if ($strArr[$i] == '.')		
		$bot = true;
}

if (count($strArr) > 2)
{
	$bot = $true;
}

$tmp=1;
$cnt=0;
for($i=count($strArr)-1;$i>=0;$i--){
	$cnt+=$tmp*($strArr[$i]-'0');
	$tmp*=10;
}
if ($bot == false)
{
	if ($cnt == 3)
		echo "True!";
	else
		echo "Input False!";
}
else 
{
	echo "False!";
}

return 0;
?>
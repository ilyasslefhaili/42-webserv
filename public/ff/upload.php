<html>
<body>

<?php 

$file_name = $_FILES['file']['name'];
$location = "./".$file_name;

if (move_uploaded_file($_FILES['file']['tmp_name'], $location)) {
	echo '<p> File uploaded successfully </p>';
} else {
	echo '<b> Error uplpoading file </b>';
}


?>

</body>
</html>
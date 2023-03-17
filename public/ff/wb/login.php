<?php
session_start();

if (!empty($_GET["action"]) && $_GET["action"] === "logout")
{
	session_unset();
	session_destroy();
}

if (!empty($_POST['fname'])) 
	$_SESSION['fname'] = $_POST['fname'];

if (!empty($_POST['lname'])) 
	$_SESSION['lname'] = $_POST['lname'];

if (!empty($_POST['email'])) {
	$_SESSION['email'] = $_POST['email'];
	// setcookie('email', $_POST['email']);
}

?>

<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Session Test</title>
	<link rel="stylesheet" href="CSS/login.css">
</head>
<body>

	
<?php if (empty($_SESSION['email'])): ?>
        <form method="POST" action="login.php">

        	<h1>Welcome to the login page:</h1>
        	       	
        		
        		<input type="text" name="fname" placeholder="your name"><br><br>
        		<input type="text" name="lname" placeholder="Your last name"><br><br>

        		<input type="text" name="email" placeholder="You email" ><br><br>       	
        		
        	    <input type="password" name="password" placeholder="your password"><br><br>        	
        	

                <td align="center"><br><input type="submit" value="Se Connecter">
                 	

        </form>
<?php else: ?>
	<h1>Profil</h1>

	<h2>
		hello , 
		<span style="color: red;"> 
			<?php 
				echo $_SESSION['fname'] . ' ' . $_SESSION['lname']; 
			?> 
		</span>
	</h2>


	<h2>
		Voici votre email: 
		<span style="color: red;"> 
			<?php 
				echo $_SESSION['email'];
			?> 
		</span>
	</h2>

	<a href="login.php?action=logout" >Logout</a>


<?php endif; ?>

</body>
</html>
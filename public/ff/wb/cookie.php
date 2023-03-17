<?php
$name=null;
if (!empty($_GET["action"]) && $_GET["action"] === "logout")
{
	unset($_COOKIE['username']);
	setcookie('username', '', time() - 3600);
}
if (!empty($_COOKIE['username'])) {
	$name = $_COOKIE['username'];
}
if (!empty($_POST['username'])) {
	setcookie('username', $_POST['username']);
	$name = $_POST['username'];
}
require 'header.php';
?>

<?php if ($name): ?>
	<h1>  Hello <?= htmlentities($name) ?> </h1>
	<a href="cookie.php?action=logout" >Logout</a>
<?php else: ?>
<main role="main" class="container">

      <div class="starter-template">
        <h1>Bootstrap starter template</h1>
        <p class="lead">Use this document as a way to quickly start any new project.<br> All you get is this text and a mostly barebones HTML document.</p>
      </div>

	  <form action="" method="post">
		<div class="form-group">
			<input class="form-control" name="username" placeholder="Enter your name">

		</div>
		<button class="btn btn-primary" > Login </buton>

		</form>
</main>
<?php endif; ?>

<?php require 'footer.php'; ?>
# Astroids
is a 2d game that allows the player to move around a 2d space in which he encounters some astroids and he must to destroy them before his ship is destroyed.

<h2>Things Applied</h2>
<ul>
	<li>I used C++ and Raylib to make this game</li>
	<li>I tried to apply some linear algebra concepts my self (using raymath helper functions) to rotate the player and translate it using Matrix Transformation, to be honset this is my first time i try this.</li>
	<li>Generating irregular shapes by using a center point to generate 8 points that has a random radius from this center point to generate astroids that are irregular</li>
	<li>Collision detection between the player and any irregular shape is not totally accurate i could have used Ray Casting but that would be much more complex than the approach i used which is quite simpler, all what i do is just caluclate the distance between any of the 3 points of the player triangle and the center point of every astroid and determine whether this distance is less than the radius of the farest point in the astroid, in other words i have made it like a bounding circle that its radius is the distance between the farest point and the center point of the astroid</li>
</ul>
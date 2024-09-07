# Astroids
is a 2d game that allows the player to move around a 2d space in which he encounters some astroids and he must to destroy them before his ship is destroyed.

<img src="Astroids.gif">

<ul>
	<li>I used C++ and Raylib to make this game</li>
	<li>I tried to apply some linear algebra concepts my self (using raymath helper functions) to rotate the player and translate it using Matrix Transformation, to be honset this is my first time i try this.</li>
	<li>Generating irregular shapes by using a center point to generate 8 points that has a random radius from this center point (all the points at the end are connected to each other to form a polygon)</li>
	<li>Collision detection between the player and any irregular shape is not totally accurate i could have used Ray Casting but that would make it much more complicated than the approach i used, all what i do is just caluclating the distance between any of the 3 points of the player triangle and the center point of every astroid and determining whether this distance is less than the radius of the farest point in the astroid, in other words i have made it like a bounding circle that its radius is the distance between the farest point and the center point of the astroid</li>
	<li>Enhanced visuals and sounds, added saucer that passes every while which will give the player extra live if he has less than 3 lives and will give him 10 points added to his score if the player hit it</li>
</ul>
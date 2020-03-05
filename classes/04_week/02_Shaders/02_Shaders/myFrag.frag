#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;
out vec4 fs_out_col;

void main()
{
	if ( 
		sqrt  
			( pow ( vs_out_pos.x, 2 ) + pow ( vs_out_pos.y, 2 ) )
		< 0.5 
	    )
	{
	
	discard; //fragment eldobása: háttérszín-törlésiszín jelenik meg
	
	}

	fs_out_col = vec4(vs_out_col, 1); //1 - homogénkoordináta
}

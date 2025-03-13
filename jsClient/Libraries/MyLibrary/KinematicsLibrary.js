window.myKinematicsUtilities = {








getPosition: function(vToB1FromI, vToB2FromB1, vToPFromB2,RB1,RB2F1) {
	
	var v1 = [0,0,0];
	var v2 = [0,0,0];	
	var v3 = [0,0,0];
	var vPosition = [0,0,0];

	
	

	vPosition = VecVecAdd(	vToB1FromI, 
				VecVecAdd(   MatVecMult(RB1,vToB2FromB1), 
				             MatVecMult(RB1,   
							MatVecMult(RB2F1,  
							vToPFromB2))));
	return vPosition;

},






getRotationMatrix: function(axis, angle) {


	var R = [[0,0,0],[0,0,0],[0,0,0]];
	
	if(axis == 1) {
		R[0][0] = 1
		R[0][1] = 0
		R[0][2] = 0
		R[1][0] = 0
		R[1][1] =  Math.cos(angle);
		R[1][2] = -Math.sin(angle);
		R[2][0] = 0
		R[2][1] =  Math.sin(angle);
		R[2][2] =  Math.cos(angle);
	}

	if(axis == 2) {
		R[0][0] = Math.cos(angle);
		R[0][1] = 0
		R[0][2] = Math.sin(angle);
		R[1][0] = 0
		R[1][1] =  1;
		R[1][2] = 0;
		R[2][0] = -Math.sin(angle);
		R[2][1] =  0;
		R[2][2] =  Math.cos(angle);

	}

	if(axis == 3) {
		R[0][0] = Math.cos(angle);
		R[0][1] = -Math.sin(angle);
		R[0][2] = 0
		R[1][0] =  Math.sin(angle);
		R[1][1] =  Math.cos(angle);
		R[1][2] = 0
		R[2][0] = 0
		R[2][1] =  0;
		R[2][2] =  1;

	}
	return R;

}


}

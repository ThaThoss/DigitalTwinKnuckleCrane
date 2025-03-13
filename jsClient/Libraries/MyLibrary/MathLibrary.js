
function MatByCons(mat, cons) {
		var mato = [[0,0,0],[0,0,0],[0,0,0]]
		mato[0][0] = cons*mat[0][0]
		mato[0][1] = cons*mat[0][1]
		mato[0][2] = cons*mat[0][2]
		mato[1][0] = cons*mat[1][0]
		mato[1][1] = cons*mat[1][1]
		mato[1][2] = cons*mat[1][2]
		mato[2][0] = cons*mat[2][0]
		mato[2][1] = cons*mat[2][1]
		mato[2][2] = cons*mat[2][2]
		return mato
}

function VecByCons(veci, cons) {
		var veco = [0,0,0]
		veco[0] = cons*veci[0]
		veco[1] = cons*veci[1]
		veco[2] = cons*veci[2]
		return veco
}

function getsign(x) {
	      if (x>= 0) {return 1}
		  if (x <0 ) {return -1}
}

function CopyMatrix(mati, RMatrix) {
		RMatrix[0][0]= mati[0][0]
		RMatrix[0][1]= mati[0][1]
		RMatrix[0][2]= mati[0][2]
		RMatrix[1][0]= mati[1][0]
		RMatrix[1][1]= mati[1][1]
		RMatrix[1][2]= mati[1][2]
		RMatrix[2][0]= mati[2][0]
		RMatrix[2][1]= mati[2][1]
		RMatrix[2][2]= mati[2][2]
}

function ZeroMat(mato) {
		mato[0][0] = 0
		mato[0][1] = 0
		mato[0][2] = 0
		mato[1][0] = 0
		mato[1][1] = 0
		mato[1][2] = 0
		mato[2][0] = 0
		mato[2][1] = 0
		mato[2][2] = 0
}





function VecVecSub(vec1, vec2)  {
		var veco = [0,0,0]
		veco[0]= vec1[0] - vec2[0]
		veco[1]= vec1[1] - vec2[1]
		veco[2]= vec1[2] - vec2[2]
		return veco
}

function makeRotation(matrix, angle, axis) {
		matrix[0][0]=1;
		matrix[0][1]=0;
		matrix[0][2]=0;
		matrix[1][0]=0;
		matrix[1][1]=1;
		matrix[1][2]=0;
		matrix[2][0]=0;
		matrix[2][1]=0;
		matrix[2][2]=1;

		if(axis == 1) {
			matrix[1][1]= Math.cos(angle);
			matrix[1][2]=-Math.sin(angle);
			matrix[2][1]= Math.sin(angle);
			matrix[2][2]= Math.cos(angle);
		}
		if(axis == 2) {
			matrix[0][0]= Math.cos(angle);
			matrix[0][2]= Math.sin(angle);
			matrix[2][0]=-Math.sin(angle);
			matrix[2][2]= Math.cos(angle);
		}
		if(axis == 3) {
			matrix[0][0]= Math.cos(angle);
			matrix[0][1]=-Math.sin(angle);
			matrix[1][0]= Math.sin(angle);
			matrix[1][1]= Math.cos(angle);
		}
}

function makeDiffRotation(matrix, angle, angularvelocity, axis) {
		matrix[0][0]=0;
		matrix[0][1]=0;
		matrix[0][2]=0;
		matrix[1][0]=0;
		matrix[1][1]=0;
		matrix[1][2]=0;
		matrix[2][0]=0;
		matrix[2][1]=0;
		matrix[2][2]=0;

		if(axis == 1) {
			matrix[1][1]= -Math.sin(angle)*angularvelocity;
			matrix[1][2]= -Math.cos(angle)*angularvelocity;
			matrix[2][1]=  Math.cos(angle)*angularvelocity;
			matrix[2][2]= -Math.sin(angle)*angularvelocity;
		}
		if(axis == 2) {
			matrix[0][0]= -Math.sin(angle)*angularvelocity;
			matrix[0][2]=  Math.cos(angle)*angularvelocity;
			matrix[2][0]= -Math.cos(angle)*angularvelocity;
			matrix[2][2]= -Math.sin(angle)*angularvelocity;
		}
		if(axis == 3) {
			matrix[0][0]= -Math.sin(angle)*angularvelocity;
			matrix[0][1]= -Math.cos(angle)*angularvelocity;
			matrix[1][0]=  Math.cos(angle)*angularvelocity;
			matrix[1][1]= -Math.sin(angle)*angularvelocity;
		}
}
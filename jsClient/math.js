
//added below
function MyVectorNorm(Veci){
var Norm=(Veci[0]*Veci[0]+Veci[1]*Veci[1]+Veci[2]*Veci[2]);
Norm=Math.sqrt(Norm);
	return Norm
}

function SkewMat(vec) {
		var mato = [[0,0,0],[0,0,0],[0,0,0]]
		mato[0][0] = 0
		mato[0][1] = -vec[2]
		mato[0][2] = vec[1]
		mato[1][0] = vec[2]
		mato[1][1] = 0
		mato[1][2] = -vec[0]
		mato[2][0] = -vec[1]
		mato[2][1] = vec[0]
		mato[2][2] = 0
		return mato
}
//added below
function MatTran(mat) {
		var mato = [[0,0,0],[0,0,0],[0,0,0]]
		mato[0][0] = mat[0][0]
		mato[0][1] = mat[1][0]
		mato[0][2] = mat[2][0]
		mato[1][0] = mat[0][1]
		mato[1][1] = mat[1][1]
		mato[1][2] = mat[2][1]
		mato[2][0] = mat[0][2]
		mato[2][1] = mat[1][2]
		mato[2][2] = mat[2][2]
		return mato
}
//added below
function MatMatMult(mat1, mat2)  {
		var mato = [[0,0,0],[0,0,0],[0,0,0]]
		mato[0][0]=mat1[0][0]*mat2[0][0] + mat1[0][1]*mat2[1][0] +mat1[0][2]*mat2[2][0]
		mato[0][1]=mat1[0][0]*mat2[0][1] + mat1[0][1]*mat2[1][1] +mat1[0][2]*mat2[2][1]
		mato[0][2]=mat1[0][0]*mat2[0][2] + mat1[0][1]*mat2[1][2] +mat1[0][2]*mat2[2][2]
		mato[1][0]=mat1[1][0]*mat2[0][0] + mat1[1][1]*mat2[1][0] +mat1[1][2]*mat2[2][0]
		mato[1][1]=mat1[1][0]*mat2[0][1] + mat1[1][1]*mat2[1][1] +mat1[1][2]*mat2[2][1]
		mato[1][2]=mat1[1][0]*mat2[0][2] + mat1[1][1]*mat2[1][2] +mat1[1][2]*mat2[2][2]
		mato[2][0]=mat1[2][0]*mat2[0][0] + mat1[2][1]*mat2[1][0] +mat1[2][2]*mat2[2][0]
		mato[2][1]=mat1[2][0]*mat2[0][1] + mat1[2][1]*mat2[1][1] +mat1[2][2]*mat2[2][1]
		mato[2][2]=mat1[2][0]*mat2[0][2] + mat1[2][1]*mat2[1][2] +mat1[2][2]*mat2[2][2]
		return mato
}
//added below
function MatMatAdd(mat1, mat2)  {
		var mato = [[0,0,0],[0,0,0],[0,0,0]]
		mato[0][0]=mat1[0][0] + mat2[0][0]
		mato[0][1]=mat1[0][1] + mat2[0][1]
		mato[0][2]=mat1[0][2] + mat2[0][2]
		mato[1][0]=mat1[1][0] + mat2[1][0]
		mato[1][1]=mat1[1][1] + mat2[1][1]
		mato[1][2]=mat1[1][2] + mat2[1][2]
		mato[2][0]=mat1[2][0] + mat2[2][0]
		mato[2][1]=mat1[2][1] + mat2[2][1]
		mato[2][2]=mat1[2][2] + mat2[2][2]
		return mato
}
//added below
function VecVecAdd(vec1, vec2)  {
		var veco = [0,0,0]
		veco[0]= vec1[0] + vec2[0]
		veco[1]= vec1[1] + vec2[1]
		veco[2]= vec1[2] + vec2[2]
		return veco
}
//added below
function MatVecMult(mat,vec) {
		var veco = [0,0,0]

		veco[0] = mat[0][0]*vec[0] + mat[0][1]*vec[1] + mat[0][2]*vec[2]
		veco[1] = mat[1][0]*vec[0] + mat[1][1]*vec[1] + mat[1][2]*vec[2]
		veco[2] = mat[2][0]*vec[0] + mat[2][1]*vec[1] + mat[2][2]*vec[2]

		return veco
}

function MyVecVecAdd(veco,vec1,vec2,nrow)  {
	for(var i =0;i<nrow;i++){
		veco[i]= vec1[i] + vec2[i];

	}
}

function MyVecVecSub(veco,vec1,vec2,nrow)  {
	for(var i =0;i<nrow;i++){
		veco[i]= vec1[i] - vec2[i];

	}
}
function zeroMatrix(nrows, ncols){

	var matrix = [];

	for(var i=0; i < nrows; i++) {
		matrix[i] = [];
		for(var j=0; j< ncols; j++) {
			matrix[i][j] = 0;
    		}
	}
	return matrix;
	


}

function zeroVector(nrow){
	var Veco=[];
	for(var i=0;i<nrow;i++){
	Veco[i]=0;
	}
	return Veco;
}
function MyCopyVector(Veco, Veci,nrow) {
		for(var i=0;i<nrow;i++){
		Veco[i]=Veci[i];
		}
}

function MyVecByCons(veci,cons,nrow) {
	var veco=zeroVector(nrow);

		for(var i=0;i<nrow;i++){
		veco[i] = cons*veci[i];
		}
		return veco
}

function MatTranspose( mato, mati, nrow, ncol) {
	for(var i = 0; i < nrow; i++) {
		for(var j = 0; j < ncol; j++) {
			mato[j][i] = mati[i][j];
		}
	}
}

function MyMatMatMult(mato, mat1, mat2, nr1, nc2, n)  {
	for(var i = 0; i < nr1; i++) {
		for(var j = 0; j < nc2; j++) {
			mato[i][j] = 0;
			for(var k = 0; k < n; k++) {		
				mato[i][j] = mato[i][j] + mat1[i][k] * mat2[k][j];
			}

		}

	}
}

function MyMatVecMult(veco, mat1, vec2, nr1, n)  {
	for(var i = 0; i < nr1; i++) {
		veco[i] = 0;
		for(var k = 0; k < n; k++) {	
			veco[i] = veco[i] + mat1[i][k] * vec2[k];
		}
	}
}





function MyMatMatSub(mato, mat1, mat2, nr1, nc2)  {
	for(var i = 0; i < nr1; i++) {
		for(var j = 0; j < nc2; j++) {		
			mato[i][j] = mat1[i][j] - mat2[i][j];
		}

	}
}



function MyMatMatAdd(mato, mat1, mat2, nr1, nc2)  {
	for(var i = 0; i < nr1; i++) {
		for(var j = 0; j < nc2; j++) {		
			mato[i][j] = mat1[i][j] + mat2[i][j];
		}

	}
}

function MyMatByCons(mato, mat1, cons, nr1, nc2)  {
	for(var i = 0; i < nr1; i++) {
		for(var j = 0; j < nc2; j++) {		
			mato[i][j] = mat1[i][j] * cons;
		}

	}
}

function MyEye(nrows, ncols){
//Only for quaratic matrices

	var matrix = zeroMatrix(nrows, ncols);

	for(var i=0; i < nrows; i++) {
		matrix[i][i]=1;
	}
	return matrix;
}


function expMRodriguez(w1, w2, w3, t, flag) {

	var normw0 = Math.sqrt(w1*w1 + w2*w2 + w3*w3);
	var O = zeroMatrix(3,3);

	var I = [[1,0,0],[0,1,0],[0,0,1]];
	var m2 = zeroMatrix(3,3);
	var m3 = zeroMatrix(3,3);
	var m4 = zeroMatrix(3,3);
	var m5 = zeroMatrix(3,3);
	var m6 = zeroMatrix(3,3);

	O[0][0] = 0;
	O[0][1] = -w3;
	O[0][2] = w2;
	O[1][0] = w3;
	O[1][1] = 0;
	O[1][2] = -w1;
	O[2][0] = -w2;
	O[2][1] = w1;
	O[2][2] = 0;

	if(normw0 >= 0.00000001) {
		var f1 = (Math.sin(normw0*t))/(normw0);
		var f2 = (1- Math.cos(normw0*t))/(normw0*normw0)
	}
	if(normw0 <= 0.00000001) {
		var f1 = 0
		var f2 = 0
	}
	MyMatByCons(m2, O, f1, 3,3) ;

	m3=MatMatMult( O, O) ;
	MyMatByCons(m4, m3, f2, 3,3) ;

	MyMatMatAdd(m5, m2,m4,3,3);
	MyMatMatAdd(m6, I,m5,3,3);


	return m6;
}

function inverse(_A) {

    var temp,
    N = _A.length,
    E = [];

    for (var i = 0; i < N; i++)
      E[i] = [];
   
    for (i = 0; i < N; i++)
      for (var j = 0; j < N; j++) {
        E[i][j] = 0;
        if (i == j)
          E[i][j] = 1;
      }
  
    for (var k = 0; k < N; k++) {
      temp = _A[k][k];
   
      for (var j = 0; j < N; j++)
      {
        _A[k][j] /= temp;
        E[k][j] /= temp;
      }
   
      for (var i = k + 1; i < N; i++)
      {
        temp = _A[i][k];
   
        for (var j = 0; j < N; j++)
        {
          _A[i][j] -= _A[k][j] * temp;
          E[i][j] -= E[k][j] * temp;
        }
      }
    }
   
    for (var k = N - 1; k > 0; k--)
    {
      for (var i = k - 1; i >= 0; i--)
      {
        temp = _A[i][k];
   
        for (var j = 0; j < N; j++)
        {
          _A[i][j] -= _A[k][j] * temp;
          E[i][j] -= E[k][j] * temp;
        }
      }
    }
   
    for (var i = 0; i < N; i++)
      for (var j = 0; j < N; j++)
        _A[i][j] = E[i][j];
    return _A;
}

function MyVectorPrinter(Veci,Nrow){

	for(var i=0;i<Nrow;i++){
		
			document.write(Veci[i]+"&emsp;");
			
	};
	document.write("<br/>");
	document.write("<br/>");
}

function MyMatrixPrinter(Matrix,Nrow,Ncol){

	for(var i=0;i<Nrow;i++){
		for(var j=0;j<Ncol;j++){
			document.write(Matrix[i][j]+"&emsp;");
			
		};
		document.write("<br/>");
	};
	document.write("<br/>");
	document.write("<br/>");
}
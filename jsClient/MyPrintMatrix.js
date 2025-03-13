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


window.myGraphicsUtilities = {

makeInertialLookAtCamera: function(Frame,camera) {
		var e1, e2, e3;
		e1 = Frame.getObjectByName("X", true);
		e2 = Frame.getObjectByName("Y", true);
		e3 = Frame.getObjectByName("Z", true);
	
		e1.lookAt(camera.position);
		e2.lookAt(camera.position);
		e3.lookAt(camera.position);
},


makeAFrame: function(factor, superscript) {

	var e1, e2, e3;
	var arrowSF = factor/ 10;
	var frameVL = factor;
	var textOffset = factor / 12
	var textScale = factor / 7



	aFrame = this.makeFrame(arrowSF,frameVL);


	e1 = this.makeAxisLabel("e", "1", superscript, 0xff0000);
	aFrame.add(e1);
	e1.position.x = frameVL + textOffset;
//	e1.lookAt(camera.position);
	e1.scale.set(textScale, textScale, textScale);



	e2 = this.makeAxisLabel("e", "2", superscript, 0x006600);
	aFrame.add(e2); 
	e2.position.y= frameVL + textOffset;
//	e2.lookAt(camera.position);
	e2.scale.set(textScale, textScale, textScale);

	e3 = this.makeAxisLabel("e", "3", superscript, 0x0000ff);
	aFrame.add(e3);
	e3.position.z = frameVL + textOffset;
//	e3.lookAt(camera.position);
	e3.scale.set(textScale, textScale, textScale);
	
	e1.name='X';
	e2.name='Y';
	e3.name='Z';
	
	return aFrame;
},

makeAxFrame: function(factor, superscript) {

	var e1, e2, e3;
	var arrowSF = factor/ 10;
	var frameVL = factor;
	var textOffset = factor / 12
	var textScale = factor / 4



	aFrame = this.makeFrame(arrowSF,frameVL);


	e1 = this.makeAxisLabel("e", "1", superscript, 0xff0000);
	aFrame.add(e1);
	e1.position.x = frameVL + textOffset;
//	e1.lookAt(camera.position);
	e1.scale.set(textScale, textScale, textScale);



	e2 = this.makeAxisLabel("e", "2", superscript, 0x006600);
	aFrame.add(e2); 
	e2.position.y= frameVL + textOffset;
//	e2.lookAt(camera.position);
	e2.scale.set(textScale, textScale, textScale);

	e3 = this.makeAxisLabel("e", "3", superscript, 0x0000ff);
	aFrame.add(e3);
	e3.position.z = frameVL + textOffset;
//	e3.lookAt(camera.position);
	e3.scale.set(textScale, textScale, textScale);
	
	e1.name='X';
	e2.name='Y';
	e3.name='Z';
	
	return aFrame;
},

makeACoord: function(factor, clabel, superscript) {

	var e1, e2, e3;
	var arrowSF = factor/ 10;
	var frameVL = factor;
	var textOffset = factor / 12
	var textScale = factor / 7



	aFrame = this.makeFrame(arrowSF,frameVL);


	e1 = this.makeAxisLabel(clabel, "1", superscript, 0xaaaaaa);
	aFrame.add(e1);
	e1.position.x = frameVL + textOffset;
//	e1.lookAt(camera.position);
	e1.scale.set(textScale, textScale, textScale);



	e2 = this.makeAxisLabel(clabel, "2", superscript, 0xaaaaaa);
	aFrame.add(e2); 
	e2.position.y= frameVL + textOffset;
//	e2.lookAt(camera.position);
	e2.scale.set(textScale, textScale, textScale);

	e3 = this.makeAxisLabel(clabel, "3", superscript, 0xaaaaaa);
	aFrame.add(e3);
	e3.position.z = frameVL + textOffset;
//	e3.lookAt(camera.position);
	e3.scale.set(textScale, textScale, textScale);
	
	e1.name='X';
	e2.name='Y';
	e3.name='Z';
	
	return aFrame;
},


makeAxCoord: function(factor, clabel, superscript) {

	var e1, e2, e3;
	var arrowSF = factor/ 10;
	var frameVL = factor;
	var textOffset = factor / 12
	var textScale = factor / 4



	aFrame = this.makeFrame(arrowSF,frameVL);


	e1 = this.makeAxisLabel(clabel, "1", superscript, 0xaaaaaa);
	aFrame.add(e1);
	e1.position.x = frameVL + textOffset;
//	e1.lookAt(camera.position);
	e1.scale.set(textScale, textScale, textScale);



	e2 = this.makeAxisLabel(clabel, "2", superscript, 0xaaaaaa);
	aFrame.add(e2); 
	e2.position.y= frameVL + textOffset;
//	e2.lookAt(camera.position);
	e2.scale.set(textScale, textScale, textScale);

	e3 = this.makeAxisLabel(clabel, "3", superscript, 0xaaaaaa);
	aFrame.add(e3);
	e3.position.z = frameVL + textOffset;
//	e3.lookAt(camera.position);
	e3.scale.set(textScale, textScale, textScale);
	
	e1.name='X';
	e2.name='Y';
	e3.name='Z';
	
	return aFrame;
},


makeJunkFrame: function(factor, superscript) {

	var e1, e2, e3;
	var arrowSF = factor/ 10;
	var frameVL = factor;
	var textOffset = factor / 12
	var textScale = factor / 7



	aFrame = this.makeFrame(arrowSF,frameVL);


	e1 = this.makeAxisLabel("e", "1", superscript, 0x000000);
	aFrame.add(e1);
	e1.position.x = frameVL + textOffset;
//	e1.lookAt(camera.position);
	e1.scale.set(textScale, textScale, textScale);



	e2 = this.makeAxisLabel("e", "2", superscript, 0x000000);
	aFrame.add(e2); 
	e2.position.y= frameVL + textOffset;
//	e2.lookAt(camera.position);
	e2.scale.set(textScale, textScale, textScale);

	e3 = this.makeAxisLabel("e", "3", superscript, 0x000000);
	aFrame.add(e3);
	e3.position.z = frameVL + textOffset;
//	e3.lookAt(camera.position);
	e3.scale.set(textScale, textScale, textScale);
	
	e1.name='X';
	e2.name='Y';
	e3.name='Z';
	
	return aFrame;
},
	


makeFrame: function(fx, dx) {

	

	var frameObject = new THREE.Object3D();
	
	var materialx = new THREE.LineBasicMaterial({ color: 0xff0000, linewidth: 1 });
	var materialy = new THREE.LineBasicMaterial({ color: 0x006600, linewidth: 1 });
	var materialz = new THREE.LineBasicMaterial({ color: 0x0000ff, linewidth: 1 });
	
	geometryx = new THREE.Geometry();
	geometryx.vertices.push(new THREE.Vector3(0, 0, 0));
	geometryx.vertices.push(new THREE.Vector3(dx, 0, 0));

	geometryy = new THREE.Geometry();
	geometryy.vertices.push(new THREE.Vector3(0, 0, 0));
	geometryy.vertices.push(new THREE.Vector3(0, dx, 0));

	geometryz = new THREE.Geometry();
	geometryz.vertices.push(new THREE.Vector3(0, 0, 0));
	geometryz.vertices.push(new THREE.Vector3(0, 0, dx));

	linex = new THREE.Line(geometryx, materialx);
	liney = new THREE.Line(geometryy, materialy);
	linez = new THREE.Line(geometryz, materialz);

	
	frameObject.add(linex);
	frameObject.add(liney);
	frameObject.add(linez);	


    	var Cx = new THREE.Mesh(new THREE.CylinderGeometry(0, 0.25, 0.5, 10, 10, false), 
		new THREE.MeshLambertMaterial({color:  0xff0000}));
    	Cx.overdraw = true;
	Cx.scale.x = fx
	Cx.scale.y = fx
	Cx.scale.z = fx
	Cx.rotation.z = -3.14159/2
	Cx.position.x = dx

    	linex.add(Cx);


  	var Cy = new THREE.Mesh(new THREE.CylinderGeometry(0, 0.25, 0.5, 10, 10, false), 
		new THREE.MeshLambertMaterial({color:  0x00aa00}));
    	Cy.overdraw = true;
	Cy.scale.x = fx
	Cy.scale.y = fx
	Cy.scale.z = fx
	Cy.rotation.z = 0
	Cy.position.y = dx
    	liney.add(Cy);
 

	var Cz = new THREE.Mesh(new THREE.CylinderGeometry(0, 0.25, 0.5, 10, 10, false), 
				new THREE.MeshLambertMaterial({color:  0x0000ff}));
    	Cz.overdraw = true;
	Cz.scale.x = fx
	Cz.scale.y = fx
	Cz.scale.z = fx
	Cz.rotation.x = 3.14159/2
	Cz.position.z = dx
    
	linez.add(Cz);

	
	return frameObject;

},
	  
movingFrameTextLooksAtCamera: function(myMovingFrame, camera, scene) {	
		var e1, e2, e3;
		e1 = myMovingFrame.getObjectByName("X", true);
		e2 = myMovingFrame.getObjectByName("Y", true);
		e3 = myMovingFrame.getObjectByName("Z", true);
	
		THREE.SceneUtils.detach( e1, myMovingFrame, scene );
		e1.lookAt(camera.position); 
		e1.updateMatrixWorld();
		THREE.SceneUtils.attach( e1, scene, myMovingFrame );
		
		THREE.SceneUtils.detach( e2, myMovingFrame, scene );
		e2.lookAt(camera.position); 
		e2.updateMatrixWorld();
		THREE.SceneUtils.attach( e2, scene, myMovingFrame );
	
		THREE.SceneUtils.detach( e3, myMovingFrame, scene );
		e3.lookAt(camera.position); 
		e3.updateMatrixWorld();
		THREE.SceneUtils.attach( e3, scene, myMovingFrame );
		
},
	
makeVector: function(fx, dx, myColor) {
	var frameObject = new THREE.Object3D();


	var aSphere = new THREE.Mesh( new THREE.SphereGeometry(0.01), new THREE.MeshLambertMaterial({color: 0xffffff}));
	aSphere.position.x = 0
	aSphere.position.y = 0
	aSphere.position.z = 0

	

	var material = new THREE.LineBasicMaterial({ color: myColor, linewidth: 1 });

	var geometry = new THREE.Geometry();
	geometry.vertices.push(new THREE.Vector3(0, 0, 0));
	geometry.vertices.push(new THREE.Vector3(dx, 0, 0));

	xline = new THREE.Line(geometry, material);
	xline.name='Marco';

	aSphere.add(xline);

   	 var C = new THREE.Mesh(new THREE.CylinderGeometry(0, 0.25, 0.5, 10, 10, false), 
		new THREE.MeshLambertMaterial({color:  myColor}));
   	C.overdraw = true;
	C.scale.x = fx
	C.scale.y = fx
	C.scale.z = fx
	C.rotation.z = -3.14159/2
	C.position.x = dx
	C.name='VectorTip';

    	xline.add(C);

	return aSphere;

},
	

makeAxisLabel: function(label1, label2, label3, color){
		
		var textex;
		var t1 = new THREE.Object3D();
	
	
		textex = this.makeText(label1, 0.4, color);
		t1.add(textex);

		text1x = this.makeText(label2, 0.2, color);
		text1x.position.x += 0.3;
		text1x.position.y -= 0.15;
		textex.add(text1x);

		text1I = this.makeText(label3, 0.2, color);
		text1I.position.x += 0.3;
		text1I.position.y += 0.15;
		textex.add(text1I);
		
	
	
  		return t1;
},


makeText: function(text, size, color) {

		
		var textx = new THREE.TextGeometry(text, {
			size: size,
			height: 0.1,
		});


		var textmaterial = new THREE.MeshFaceMaterial( [ 
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.FlatShading } ), // front
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.SmoothShading } ) // side
		] );

		return new THREE.Mesh(textx, textmaterial);
		
},

makeSingleLabel(text, size, color) {

		var texta = new THREE.TextGeometry(text, {
			size: size,
			height: 0.1,
		});
		
		
	
		var textmaterial = new THREE.MeshFaceMaterial( [ 
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.FlatShading } ), // front
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.SmoothShading } ) // side
		] );

	
		var myFullText = new THREE.Object3D();

		var myTextObject1 = new THREE.Mesh(texta, textmaterial);
	

		myFullText.add(myTextObject1);
		
	

		return myFullText;

},


makeLabelWithSuperscript: function(text1, text2, size, color) {

		
		var texta = new THREE.TextGeometry(text1, {
			size: size,
			height: 0.1,
		});
		
		var textb = new THREE.TextGeometry(text2, {
			size: size/2,
			height: 0.05,
		});

		

		var textmaterial = new THREE.MeshFaceMaterial( [ 
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.FlatShading } ), // front
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.SmoothShading } ) // side
		] );

	
		var myFullText = new THREE.Object3D();

		var myTextObject1 = new THREE.Mesh(texta, textmaterial);
		var myTextObject2 = new THREE.Mesh(textb, textmaterial);

		myFullText.add(myTextObject1);
		myFullText.add(myTextObject2);
		
		myTextObject2.position.y += 0.6;
		myTextObject2.position.x += 0.8;

		return myFullText;
		
},


makeVectorName: function(text1, text2, size, color) {

		
		var texta = new THREE.TextGeometry(text1, {
			size: size,
			height: 0.1,
		});
		
		var textb = new THREE.TextGeometry(text2, {
			size: size/2,
			height: 0.05,
		});

		

		var textmaterial = new THREE.MeshFaceMaterial( [ 
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.FlatShading } ), // front
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.SmoothShading } ) // side
		] );

	
		var myFullText = new THREE.Object3D();

		var myTextObject1 = new THREE.Mesh(texta, textmaterial);
		var myTextObject2 = new THREE.Mesh(textb, textmaterial);

		myFullText.add(myTextObject1);
		myFullText.add(myTextObject2);
		
		myTextObject2.position.x += 0.6;
		myTextObject2.position.y -= 0.4;

		return myFullText;
		
},

makeBodyName: function(text1, text2, size, color) {

		
		var texta = new THREE.TextGeometry(text1, {
			size: size,
			height: 0.1,
		});
		
		var textb = new THREE.TextGeometry(text2, {
			size: size/2,
			height: 0.05,
		});

		

		var textmaterial = new THREE.MeshFaceMaterial( [ 
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.FlatShading } ), // front
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.SmoothShading } ) // side
		] );

	
		var myFullText = new THREE.Object3D();

		var myTextObject1 = new THREE.Mesh(texta, textmaterial);
		var myTextObject2 = new THREE.Mesh(textb, textmaterial);

		myFullText.add(myTextObject1);
		myFullText.add(myTextObject2);
		
		myTextObject2.position.x += 1;
		myTextObject2.position.y += 1;

		return myFullText;
		
},



makeSE3MName: function(text1, text2, text3, size, color) {

		

		var texta = new THREE.TextGeometry(text1, {
			size: size,
			height: 0.1,
		});
		
		var textb = new THREE.TextGeometry(text2, {
			size: size/2,
			height: 0.05,
		});
		var textbp = new THREE.TextGeometry(text2, {
			size: size/2,
			height: 0.05,
		});
		var textc = new THREE.TextGeometry(text3, {
			size: size/2,
			height: 0.05,
		});

		var textmaterial = new THREE.MeshFaceMaterial( [ 
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.FlatShading } ), // front
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.SmoothShading } ) // side
		] );

	
		var myFullText = new THREE.Object3D();

		var myTextObject1 =  new THREE.Mesh(texta, textmaterial);
		var myTextObject2 =  new THREE.Mesh(textb, textmaterial);
		var myTextObject2p = new THREE.Mesh(textbp, textmaterial);
		var myTextObject3 =  new THREE.Mesh(textc, textmaterial);

		myFullText.add(myTextObject1);
		myFullText.add(myTextObject2);
		myFullText.add(myTextObject2p);
		myFullText.add(myTextObject3);
		
		myTextObject2.position.x += 0.5;
		myTextObject2.position.y += 0.6;

		myTextObject2p.position.x += 2.4;
		myTextObject2p.position.y += 0.6;

		myTextObject3.position.x += 2.4;
		myTextObject3.position.y -= 0.3;

		return myFullText;
		
},



makeSE3VName: function(text1, text2, text3, size, color) {

		

		var texta = new THREE.TextGeometry(text1, {
			size: size,
			height: 0.1,
		});
		
		var textb = new THREE.TextGeometry(text2, {
			size: size/2,
			height: 0.05,
		});

		var textc = new THREE.TextGeometry(text3, {
			size: size/2,
			height: 0.05,
		});

		var textmaterial = new THREE.MeshFaceMaterial( [ 
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.FlatShading } ), // front
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.SmoothShading } ) // side
		] );

	
		var myFullText = new THREE.Object3D();

		var myTextObject1 =  new THREE.Mesh(texta, textmaterial);
		var myTextObject2 =  new THREE.Mesh(textb, textmaterial);
		var myTextObject3 =  new THREE.Mesh(textc, textmaterial);

		myFullText.add(myTextObject1);
		myFullText.add(myTextObject2);
		myFullText.add(myTextObject3);
		
		myTextObject2.position.x += 0.4;
		myTextObject2.position.y += 0.5;

		myTextObject3.position.x += 0.4;
		myTextObject3.position.y -= 0.3;

		return myFullText;
		
},














makeSE3IName: function(text1, text2,size, color) {

	

		var texta = new THREE.TextGeometry(text1, {
			size: size,
			height: 0.1,
		});
		
		var textb = new THREE.TextGeometry(text2, {
			size: size/2,
			height: 0.05,
		});
	

		var textmaterial = new THREE.MeshFaceMaterial( [ 
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.FlatShading } ), // front
			new THREE.MeshPhongMaterial( { color: color, shading: THREE.SmoothShading } ) // side
		] );

	
		var myFullText = new THREE.Object3D();

		var myTextObject1 =  new THREE.Mesh(texta, textmaterial);
		var myTextObject2 =  new THREE.Mesh(textb, textmaterial);
		

		myFullText.add(myTextObject1);
		myFullText.add(myTextObject2);
		
		
		myTextObject2.position.x += 0.8;
		myTextObject2.position.y += 0.5;

	

		return myFullText;
		
},



makeTextLookAtCamera: function(camera, text1 ) {
	
		text1.lookAt(camera.position);
	
}

}



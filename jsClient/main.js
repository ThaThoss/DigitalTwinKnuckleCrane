var damping = 25000

var abcd = 0;



class myTime {
    constructor () {

        this.minute = 60000;
        this.hour = this.minute * 60;
        this.day = this.hour * 24;
        this.year = this.day * 365;

        const date = new Date(); 
        this.startTime = date.getTime()/1000;//Seconds
        this.timeElapsed = date.getTime()/1000 - this.startTime;

    }

    getElapsed() {
        const date = new Date(); 
        return this.timeElapsed = date.getTime()/1000 - this.startTime;
    }
}

var myTimes = {

    tend: 60,
    dt: 0.005,
    nstop: 0

}

var myPhysics = {
    m1: 9000, //kg
    m2: 160,
    m3: 150,
    m4: 150,

    //------------------------------------------------
    // Lenghts in m

    Lps: 23, // width of ship
    Lsb: 110, // lenght of ship
    Lh: 30, // max height of ship

    // from C1 to J21

    b2: -7.5, // Should it be minus?
    L21: -11.9,
    h21: 13.5,

    // from J21 to C2

    L22: 0, //-0.14
    h22: 2.6,

    // from C2 to J32

    L31: 0, //0.14,
    h31: 3.5,

    // from J32 to C3 

    L32: -10.7,
    h32: 0, //-0.2,

    // from C3 to J43

    L41: -11.3,
    h41: 0, //0.2,

    // from J43 to C4

    L42: 4.1,
    h42: -2.4,

    damp: damping,
    numLinks: 4,

    x: 0,
    y: 0,
    z: 0
}


var myArrayData = {
    spatialDim: 3,
    numGenCoord: 6,
    numLocalSpace: 6,
    numElemProp: 21
}



var myMotorControls = {
    FACTOR1: 1,
    FACTOR2: 1,
    FACTOR3: 1,
    FACTOR4: 1,
    FACTOR5: 1
}



var myGeneralizedData = {
    time: [],
    w1: [],
    w2: [],
    w3: [],
    q4: [],
    q5: [],
    q6: [], // added
    Mom1: [], // name changed from q6 to Mom1, also changed names elsewhere in code
    Mom2: [], // name changed from q7 to Mom2
    Mom3: [] // added
}

var myPosData = {

    R:  [1,0,0,0,1,0,0,0,1],
    th1:0,
    th2:0,
    th3:0

}


var myDataFromServer = {
    socket: [],//new WebSocket('ws://localhost:8081'),
    data: []
}
var dataForServer = {

    reset: 0,
    

}



$(document).ready(function() {
    "use strict";

    var myProcessingLight;

    var icount = 0;
    var rot1 = MyEye(3,3);
    makeRotation(rot1, Math.PI/2, 3);

    var rot2 = MyEye(3,3);
    makeRotation(rot2, 3*Math.PI/2, 1);

    var rotFromKnuckleToAni = MyEye(3,3);
    MyMatMatMult(rotFromKnuckleToAni, rot1, rot2, 3, 3, 3);
    var rotFromKnuckleToAniTranspose = [
        [rotFromKnuckleToAni[0][0],rotFromKnuckleToAni[1][0],rotFromKnuckleToAni[2][0]],
        [rotFromKnuckleToAni[0][1],rotFromKnuckleToAni[1][1],rotFromKnuckleToAni[2][1]],
        [rotFromKnuckleToAni[0][2],rotFromKnuckleToAni[1][2],rotFromKnuckleToAni[2][2]]
    ];

    //Variables for sliders
    var sliderT = $("#mySliderT").slider();
    var sliderMB = $("#mySliderMB").slider();
    var sliderOB = $("#mySliderOB").slider();
    //----------------------------------------


    var myAnimate = 0;
    var connected = 0;
    var socket =  new WebSocket('ws://127.0.0.1:8081');/*ws://158.37.70.146:8081*/

    socket.addEventListener('open', (event) => {
        console.log('WebSocket connection opened:', event);
    });
    
    socket.addEventListener('message', (event) => {
    
        console.log('Message from server:', event.data);
        // Assuming the server sends a JSON string
        myDataFromServer.data = JSON.parse(event.data);
        console.log('Received json string:', event.data);
        myAnimate = 1;
        
    });
    
    // Connection closed
    socket.addEventListener('close', (event) => {
        console.log('WebSocket connection closed:', event);
        connected = 0;
        myAnimate = 0;
    });
    
    // Connection error
    socket.addEventListener('error', (event) => {
        console.error('WebSocket connection error:', event);
    });
    if(socket.readyState === socket.OPEN)
    {
        connected = 1;
    }
    




    var camera, scene, renderer, container, controls;

    var myClock = new myTime();


    var ASPECT = 0.6;

    var o1, o2, o3, o4; 

  
    var cameraPosition = new THREE.Vector3(25, 25, 25);

    $("#myStartButton").prop('disabled', false);
    $("#myResetButton").prop('disabled', true);

    


    $(function() {
        $("#myStartButton").click(function() {

            $("#myStartButton").prop('disabled', true);
            $("#myResetButton").prop('disabled', false);

            //get_motors_from_web();

            //GetAngles()
            //Set the factors from the sliders
            myMotorControls.TowerPos = get_Slider4TowerPos();
            myMotorControls.MainboomPos = get_Slider4MainboomPos();
            myMotorControls.OuterboomPos = get_Slider4OuterboomPos();
            //---------------------------------------------------         
            icount = 0;

        });
    });


    $(function() {
        $("#myResetButton").click(function() {
            myAnimate = 0;

            $("#myStartButton").prop('disabled', false);
            $("#myResetButton").prop('disabled', true);
            dataForServer.reset = 1;



            renderer.render(scene, camera);
            requestAnimationFrame(animate);


            o1.rotation.x = 0;
            o1.rotation.y = 0;
            o1.rotation.z = 0;

            o2.rotation.y = 0; // changed rotation from y to x
            o3.rotation.x = 0;
            o4.rotation.x = 0; // added 

        });
    });

    $(function() {
        $("#myReconButton").click(
            function() {
                reConnect();
            });
                
    });

    async function sleep(ms) {
        await new Promise(resolve => setTimeout(resolve, ms));
    }

    function reConnect() {
        socket = new WebSocket('ws://192.168.50.217:8081');
    }

    function get_Slider4TowerPos() {
        var T = parseFloat(sliderT.val());
        if (isNaN(T)) {
            return 0;
        }
        return T;
    }

    function get_Slider4MainboomPos() {
        var MB = parseFloat(sliderMB.val());
        if (isNaN(MB)) {
            return 0;
        }
        return MB;
    }

    function get_Slider4OuterboomPos() {
        var OB = parseFloat(sliderOB.val());
        if (isNaN(OB)) {
            return 0;
        }
        return OB;
    }

    function init() {

        var dimensions = rendererDimensions();

        renderer = new THREE.WebGLRenderer();
        renderer.setSize(dimensions.width, dimensions.height);
        renderer.setClearColor(0xffffff, 1.0);

        container = document.getElementById("myCanvas");
        container.appendChild(renderer.domElement);

        scene = new THREE.Scene();
        camera = new THREE.PerspectiveCamera(45, dimensions.width / dimensions.height, 1, 2000);
        camera.position.copy(cameraPosition);
        camera.up.set(0, 1, 0)
        camera.lookAt(0, 1, 0);

        var light = new THREE.SpotLight();
        light.position.set(100, 100, 100);
        scene.add(light);

        var light1 = new THREE.SpotLight();
        light1.position.set(-100, -100, 100);
        scene.add(light1);

        var loader = new THREE.OBJMTLLoader();

        //---------------------------------------------------
        // Change names of files to be loaded

        o1 = new THREE.Object3D();
        loader.load('models/Vessel.obj', 'models/Vessel.mtl', function(object) { o1.add(object); });

        o2 = new THREE.Object3D();
        loader.load('models/Tower.obj', 'models/Tower.mtl', function(object) { o2.add(object); });


        o3 = new THREE.Object3D();
        loader.load('models/Mainboom.obj', 'models/Mainboom.mtl', function(object) { o3.add(object); });

        o4 = new THREE.Object3D();
        loader.load('models/Outerboom.obj', 'models/Outerboom.mtl', function(object) { o4.add(object); }); // Add correct filename


        scene.add(o1);
        o1.add(o2);
        o2.add(o3);
        o3.add(o4); // added


        o2.position.x = -7.5 / 4.97
        o2.position.y = 13.5 / 4.97
        o2.position.z = -11.9 / 4.97

        o3.position.y = 6.1 / 4.97
        o3.position.z = 0

        o4.position.y = 0
        o4.position.z = -22 / 4.97

        // b2 : -7.5,
        // L21 : 11.9,
        // h21 : 13.5,
        //4.97
        //---------------------------------------------------------------



        var geometry = new THREE.PlaneGeometry(1000, 1000, 32);
        var material = new THREE.MeshBasicMaterial({ color: 0x0C4D69, transparent: true, opacity: 0.5, side: THREE.DoubleSide });
        var plane = new THREE.Mesh(geometry, material);
        plane.rotation.x = 3.14159 / 2
        plane.position.y = -1.65
        scene.add(plane);


        var geometry = new THREE.SphereGeometry(1, 32, 32);
        var material = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
        var sphere = new THREE.Mesh(geometry, material);
        sphere.position.y = -5
        scene.add(sphere);


        var geometry = new THREE.SphereGeometry(0.5, 32, 32);
        var material = new THREE.MeshBasicMaterial({ color: 0xff0000 });
        myProcessingLight = new THREE.Mesh(geometry, material);
        myProcessingLight.position.x = 0
        myProcessingLight.position.y = 10
        myProcessingLight.position.z = 0
        scene.add(myProcessingLight);


        controls = new THREE.OrbitControls(camera, renderer.domElement);
        controls.rotateSpeed = 1.0;
        controls.zoomSpeed = 1.0;
        controls.panSpeed = 2.;
        controls.minDistance = 10 * 0.5;
        controls.maxDistance = 10 * 1000;

        window.addEventListener('resize', onWindowResize, false);

        renderer.render(scene, camera);

        requestAnimationFrame(animate);
    }

    //----------------------------------------------------------------------------------------------------
    function animate() {
        

       // get_motors_from_web()

        o2.rotation.y = get_Slider4TowerPos();    
        o3.rotation.x = get_Slider4MainboomPos();
        o4.rotation.x = get_Slider4OuterboomPos();

        myProcessingLight.position.x = -7.5/4.97;
        myProcessingLight.position.y = 14.467/4.97;//14.717/4.97;
        myProcessingLight.position.z = -24.79/4.97;//-25.7/4.97;


        myProcessingLight.material.color = new THREE.Color(0x00ff00);

        if (myAnimate == 1) {

            myProcessingLight.material.color = new THREE.Color(0xffff00);

            myProcessingLight.position.x = -myDataFromServer.data.globalPosPoint[1]/4.97;
            myProcessingLight.position.y = myDataFromServer.data.globalPosPoint[2]/4.97;
            myProcessingLight.position.z = -myDataFromServer.data.globalPosPoint[0]/4.97;

            //R1 is sent as absolute rotation, rotate back, then forward again
            o1.applyMatrix(
                new THREE.Matrix4(
                    myPosData.R[0], myPosData.R[3], myPosData.R[6], 0,
                    myPosData.R[1], myPosData.R[4], myPosData.R[7], 0,
                    myPosData.R[2], myPosData.R[5], myPosData.R[8], 0,
                                                           0, 0, 0, 1)
            );

            MyCopyVector(myPosData.R, myDataFromServer.data.R1,9);
            var tempM = [
                [myDataFromServer.data.R1[0], myDataFromServer.data.R1[1], myDataFromServer.data.R1[2]],
                [myDataFromServer.data.R1[3], myDataFromServer.data.R1[4], myDataFromServer.data.R1[5]],
                [myDataFromServer.data.R1[6], myDataFromServer.data.R1[7], myDataFromServer.data.R1[8]]
            ];
            var tempM2=MyEye(3,3);

            MyMatMatMult(tempM2, tempM,rotFromKnuckleToAni, 3, 3, 3);
            MyMatMatMult(tempM, rotFromKnuckleToAniTranspose,tempM2, 3, 3, 3);
            
            myPosData.R[0] = tempM[0][0];
            myPosData.R[1] = tempM[0][1];
            myPosData.R[2] = tempM[0][2];

            myPosData.R[3] = tempM[1][0];
            myPosData.R[4] = tempM[1][1];
            myPosData.R[5] = tempM[1][2];

            myPosData.R[6] = tempM[2][0];
            myPosData.R[7] = tempM[2][1];
            myPosData.R[8] = tempM[2][2];

            o1.applyMatrix(
                new THREE.Matrix4(
                    myPosData.R[0], myPosData.R[1], myPosData.R[2], 0,
                    myPosData.R[3], myPosData.R[4], myPosData.R[5], 0,
                    myPosData.R[6], myPosData.R[7], myPosData.R[8], 0,
                                                           0, 0, 0, 1)
            );

                document.getElementById("demo").innerHTML = myClock.getElapsed();

                o2.rotation.y = myDataFromServer.data.angles[0]; // changed y to z
                o3.rotation.x = -myDataFromServer.data.angles[1];
                o4.rotation.x = -myDataFromServer.data.angles[2]; // added

        }
        if(socket.readyState === socket.OPEN)
        {
            console.log('socket open:');
            connected = 1;            
        }else{
            connected = 0;
            console.log("socket closed");
        }
        
        if(connected){
            const myJSON = JSON.stringify(dataForServer);
            console.log('Sending message to server:', myJSON);          
            socket.send(myJSON);
        }

        

        controls.update();
        renderer.render(scene, camera);
        sleep(33);
        requestAnimationFrame(animate);
        
        
    }

    function rendererDimensions() {
        var width = Math.min(window.innerWidth, 960);
        var height = width * ASPECT;
        return {
            width: width,
            height: height
        }
    }

    function onWindowResize() {

        var dimensions = rendererDimensions();
        renderer.setSize(dimensions.width, dimensions.height);

        camera.aspect = dimensions.width / dimensions.height;
        camera.updateProjectionMatrix();


    }

    init();

});

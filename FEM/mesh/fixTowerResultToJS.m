
function posForBoom = fixTowerResultToJS()

    
    
    filename = "TowerMesh.dat";
    fileID = fopen(filename,'r');
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    nElements = 2460;
    nNodes = 2566;
    nNodeElement=4;
    AllElements=fscanf(fileID,"%d %d %d %d %d %d\n",[6,nElements])';
    Elements= [AllElements(:,1:5),zeros(nElements,1)];
    tline = fgetl(fileID);
    Nodes=fscanf(fileID,"%d %f %f\n",[3,nNodes])';
    Nodes = Nodes(:,2:3);
    
    
    [displacements,vonMieses] = getDiplacementsTower();
    displNodes = Nodes+displacements*200;
    
    
    
    
    
    NodesForEndPos = [502,504];
    
    posForBoom = [displNodes(NodesForEndPos(1)+1,1),displNodes(NodesForEndPos(2)+1,2)];
    
    
    %% Plot mesh
    f1 = figure(1);
    clf(f1,'reset');
 hold on
    for i=1:nElements
        for j=1:nNodeElement-1
    
            node = Elements(i,j+1:j+2)+1;     
            xCoords=displNodes(node,1);
            yCoords=displNodes(node,2);
           
            plot(xCoords,yCoords,'-b')
    
    
    
        end
        node = Elements(i,[5,2])+1;
        xCoords=displNodes(node,1);
        yCoords=displNodes(node,2);
      
        plot(xCoords,yCoords,'-b')
       
    
    end

    
    % for i=1:nElements
    %   fprintf("[%g, %g],\n",displNodes(i,1),displNodes(i,1))
    % end
    % 
    
    % for i=1:nElements
    %     fprintf("[");
    %     for j=1:3
    %         fprintf("%u, ",AllElements(i,j));
    %     end
    %     fprintf("%u],\n",AllElements(i,4));
    % end

    saveArrayAsJavaScript(Elements(:,2:end-1)," getTowerElements", "getTowerElements.js");
    saveArrayAsJavaScript(displNodes," getTowerNodes", "getTowerNodes.js");
    save1DArrayAsJavaScript(vonMieses," getTowerVonMieses", "getTowerVonMieses.js");
end
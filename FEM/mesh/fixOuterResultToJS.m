function fixOuterResultToJS(xy,theta)
    
    filename = "outerBoomMesh.dat";
    fileID = fopen(filename,'r');
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    nElements = 1328;
    nNodes = 1468;
    nNodeElement=4;
    AllElements=fscanf(fileID,"%d %d %d %d %d %d\n",[6,nElements])';
    AllElements = AllElements(:,2:5);
    tline = fgetl(fileID);
    Nodes=fscanf(fileID,"%d %f %f\n",[3,nNodes])';
    Nodes = Nodes(:,2:3);
    Nodes = Nodes.*3;
    
    holecoord = [6.21,3.3];
    moveTo = -holecoord;
    moveToSizeNodes = ones(size(Nodes()));
    moveToSizeNodes(:,1) = moveToSizeNodes(:,1).*(moveTo(1));
    moveToSizeNodes(:,2) = moveToSizeNodes(:,2).*(moveTo(2));
    Nodes = Nodes+moveToSizeNodes;
    
    
    [displacements,vonMieses] = getOuterDispAndVonMieses();
    
    R = [cosd(theta),-sind(theta);
         sind(theta), cosd(theta)];
    
    displNodes = Nodes+displacements*50;
    for i=1:size(Nodes,1)
    
        tempNode =  R*displNodes(i,:)';
        displNodes(i,:) = tempNode';
    
    end
    
        
    moveToSizeNodes = ones(size(Nodes()));
    moveToSizeNodes(:,1) = moveToSizeNodes(:,1).*xy(1);
    moveToSizeNodes(:,2) = moveToSizeNodes(:,2).*xy(2);
    displNodes = (displNodes+moveToSizeNodes);
    hold on
    %% Plot mesh
    for i=1:nElements
        for j=1:nNodeElement-1
    
            node = AllElements(i,j:j+1)+1;     
            xCoords=displNodes(node,1);
            yCoords=displNodes(node,2);
    
            plot(xCoords,yCoords,'-b')
    
    
    
        end
        node = AllElements(i,[4,1])+1;
        xCoords=displNodes(node,1);
        yCoords=displNodes(node,2);
    
        plot(xCoords,yCoords,'-b')
    
    
    end

    
    % for i=1:nElements
    %   fprintf("[%g, %g],\n",displNodes(i,1),displNodes(i,1))
    % end
    
    
    % for i=1:nElements
    %     fprintf("[");
    %     for j=1:3
    %         fprintf("%u, ",AllElements(i,j));
    %     end
    %     fprintf("%u],\n",AllElements(i,4));
    % end

    saveArrayAsJavaScript(AllElements," getOuterBoomElements", "getOuterBoomElements.js");
    saveArrayAsJavaScript(displNodes," getOuterBoomNodes", "getOuterBoomNodes.js");
    save1DArrayAsJavaScript(vonMieses," getOuterBoomVonMieses", "getOuterBoomVonMieses.js");

end
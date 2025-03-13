function posForOuter = fixBoomResultToJS(xy,theta)
    

    
    
    
    filename = "mainBoomMesh.dat";
    fileID = fopen(filename,'r');
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    tline = fgetl(fileID);
    nElements = 2220;
    nNodes = 2397;
    nNodeElement=4;
    AllElements=fscanf(fileID,"%d %d %d %d %d %d\n",[6,nElements])';
    Elements= [AllElements(:,1:5),zeros(nElements,1)];
    tline = fgetl(fileID);
    Nodes=fscanf(fileID,"%d %f %f\n",[3,nNodes])';
    Nodes = Nodes(:,2:3);


    
    [displacements,vonMieses] = getDiplacementsBoom();
    
    
    holecoord = [0.2,0.5];
    
    %Move to 0,0
    moveTo = -holecoord;
    moveToSizeNodes = ones(size(Nodes()));
    moveToSizeNodes(:,1) = moveToSizeNodes(:,1).*moveTo(1);
    moveToSizeNodes(:,2) = moveToSizeNodes(:,2).*moveTo(2);
    Nodes = Nodes+moveToSizeNodes;

    R = [cosd(theta),-sind(theta);
        sind(theta), cosd(theta)];    
    for i=1:size(Nodes,1)
    
        tempNode =  R*Nodes(i,:)';
        Nodes(i,:) = tempNode';
    
    end
    
    displNodes = Nodes+displacements*25;


    %Move to joint
    moveToSizeNodes = ones(size(Nodes()));
    moveToSizeNodes(:,1) = moveToSizeNodes(:,1).*xy(1);
    moveToSizeNodes(:,2) = moveToSizeNodes(:,2).*xy(2);
    displNodes = (displNodes*3+moveToSizeNodes);
    

    
    
    
    NodesForEndPos = [493,497];
    
    posForOuter = [ 0.5*(displNodes(NodesForEndPos(1)+1,1) + displNodes(NodesForEndPos(2)+1,1)),...
                    0.5*(displNodes(NodesForEndPos(1)+1,2) + displNodes(NodesForEndPos(2)+1,2))];
    displNodes(86 + 1,:)
    displNodes(1832 + 1,:)
    %% Plot mesh
    figure(1);

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
    
    
    % for i=1:nElements
    %     fprintf("[");
    %     for j=1:3
    %         fprintf("%u, ",AllElements(i,j));
    %     end
    %     fprintf("%u],\n",AllElements(i,4));
    % end

    saveArrayAsJavaScript(Elements(:,2:end-1)," getBoomElements", "getBoomElements.js");
    saveArrayAsJavaScript(displNodes," getBoomNodes", "getBoomNodes.js");
    save1DArrayAsJavaScript(vonMieses," getBoomVonMieses", "getBoomVonMieses.js");
end

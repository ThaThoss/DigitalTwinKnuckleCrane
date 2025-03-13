function elementPlotter(Elements,Nodes,elNr)
hold on

    for j=1:3
    node = Elements(elNr,j+1:j+2)+1;     
        xCoords=Nodes(node,2);
        yCoords=Nodes(node,3);
        

       plot(xCoords,yCoords,'-r')
       plot(Nodes(node(1),2),Nodes(node(1),3),'*')
       
     
       
    end
    node = Elements(elNr,[5,2])+1;
    xCoords=Nodes(node,2);
    yCoords=Nodes(node,3);
    
    plot(xCoords,yCoords,'-r')
    nodeNr = node(1);
    plot(Nodes(nodeNr,2),Nodes(nodeNr,3),'*')
    hold off
    
end
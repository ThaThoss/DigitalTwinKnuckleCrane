

    elNr = elementProblemNr(i)
    for j=1:3
    node = Elements(elNr,j+1:j+2)+1;     
        xCoords=Nodes(node,2);
        yCoords=Nodes(node,3);
        if(xCoords(1)>4.2)

       plot(xCoords,yCoords,'-r')
       plot(Nodes(node(1),2),Nodes(node(1),3),'*')
       Nodes(node(1),:)
       listProbElem(n) = elNr;
       n=n+1;

        end
       
    end
    node = Elements(elNr,[5,2])+1;
    xCoords=Nodes(node,2);
    yCoords=Nodes(node,3);
    if(xCoords(1)>4.2)
    plot(xCoords,yCoords,'-r')
    nodeNr = node(1);
    plot(Nodes(nodeNr,2),Nodes(nodeNr,3),'*')
    end
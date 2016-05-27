//wot

for i = 1:Nvectsbord
    
    disp(DunBordGauche(u,1,16,h,vectsgauche(i,:)) - Hetoile(vectsgauche(i,:),drondY))
    //disp(Hetoile(vectsgauche(i,:),drondY))
end

Számítógépes grafika - 2019/20 - Tavaszi félév

Szabó Dávid - sasasoft@inf.elte.hu

------------
|  		   |
|  11. hét |
|		   |
------------

FBO:
	- framebufferobject -> color buffer (textúra, szín) + depthbuffer (mélységértékek)
	- active framebuffer object-be rajzol (nem backbufferbe)
	
Módszer:
	- új FBO-ba rajzolunk a render fázisban
	- majd visszaállunk a régire (alap FBO-ra) és rajzolnuk egy teljes képernyőt lefedő színlapot
	- majd az új FBO-ban lévő texture alapján rajzoljunk a régibe (a postprocessshader segítségével)
	
	
------------
|  		   |
|  9. hét  |
|		   |
------------

Pyshics:
	- részecskerendszer:
		-- pici entitásokból:
			--- sebesség
			--- pozíció
			--- de lehet geometriája, forgása 

Transparency:
	- kirajzoljuk barna suzanne-t, a falat, majd a zöld suzanne-t 
	- barna Suzanne bekerül a mélység bufferbe, majd az áttetsző falé is, majd a zöldé
	
	- másik irányból: barna Suzanne-t először, majd a falat (barna Suzanne néhány fragmensnél kitakarja a falat) 
	
	- megoldás: rajzoljuk ki a nem áttetsző objektumokat, majd az áttetszőket:
		-- a fal kódját a két suzanne rajzolása után tesszük
		
	- kamera szerint sorrendbe: tenni az áttetszőket
	

	- ezen kívül, amire szükség van az áttetszőség működéséhez (3 lépés): 
		!! glEnable(GL_BLEND);
			(itt a projektben az initben van, de közvetlenül a rajzoláskor sokkal szebb lenne)
			--- mikor bekapcsoljuk a blendinget, minden fragmensre számítást fog végezni a rendszer a háttérben:
				---- a source (fragmens shader által előállított eredmény)-t hogyan írjuk be a már kirajzolt fragmensek közé
		
==> 	!!glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  => lineáris interpoláció		
			fragmens shader után új szerelőszalag rész fog bekapcsolni: blend merge (az újonnan kirajzolandó fragmenseket mossa össze a jelenleg backbufferben lévő fragmensekkel) 
		
			// GL_SRC_ALPHA => faktor annak amit a fragmens shader előállít
			// GL_ONE_MINUS_SRC_ALPHA => faktor annak ami már benne van a backbufferben
		
		!! fal kirajzolásakor: kikapcsolni a hátlapeldobást, majd az áttetsző objektumok kirajzolása után visszakapcsolása
		
	

------------
|  		   |
|  8. hét  |
|		   |
------------

01_OGLBase:
	- skybox stb.
	- új geometriát szeretnénk áthozni (01_Normals):
		-- VAO és VBO
		
	- új grafikai modell behozása
	
	- skybox magyarázat:
		-- kirajzolni oda, ahova még rajzolhatunk ki (távoli vágósík)
		-- mélységi teszt
		-- használata:
			--- nem csak megjelenítés, hanem tükrözödés (environment mapping)


------------
|  		   |
|  7. hét  |
|		   |
------------

Óraismétlés:
	- fényforrás típusok:
		-- irányfényforrás: minden sugár ugyanazzal az irányvektorral rendelkezik (párhuzamosak egymásra) - valóságban nincs jó példa
		-- pontfény: pl. egy villanykörte (van egy pozíció, ahonnan a világ minden irányába szórja a fényt)
		-- spot:
			--- van iránya
			--- van egy pozíciója
			--- van nyílásszöge

	- olyan messze vagyunk a naptól, hogy párhuzamosnak érzékeljük a napot, és ezért irányfényforrással fogjuk szimulálni a Napot, 
	nem érdekel minket, hogy hol van a Nap pozíciója
	
	- diffúz intenzitás kiszámítása:
		-- vektor skaláris szorzás 
			--> ha teljesen párhuzamos a felületi normális és a becsapodófény irányvektora (100% megvilágítottság)
			--> ha a fénysugár egyre merőlegesebb lesz akkor egyre kisebb a megvilágítottság
			
			számítása: 
				- a fénysugárnak kell venni (-1) - szeresét
				- felületi normális számításához pedig kell a világtranszformáció inverz transzponáltja
				
	- spekuláris komponens számítása:
		-- valójában egy r(eflect) irányvektor felületi pontból (fragmensből) a kamerába/kamera felé mutat
			

--------

Előző óra:
- körbefordulást meggátolni:

	- megoldás: A forgási szög leszorítása
		-- m_v = glm::clamp(m_v, 0.01f, 3.13f);
		
01_Normals:
	- fényforrás típusok:
		-- phong árnyalás:
			--- fénykomponensekből áll: 
			ambient (azt zárjuk ki hogy soha semmi ne legyen teljesen fekete) 
			+ diffuse ()
			+ specular ()
			
		-- pontfény
		-- nap eléggé messze van, és így kb. a sugarai eléggé párhuzamosak
		
		(kitekintő: softbox)
		
	- ambient kiszámítása (myFrag.frag)
	
	- felületi normális:
	
	
	
	@ szög: l (fénysugár) és n(ormál vektor) között
	
	(r vektor: visszaverődés)
	
		 ^
  -l \   | n
	  \@ |   / r
	   \@|  /
 	  / \|-/-/
	 /   x  /
	/------/

	normálvektor kiszámítás: gömb az egyetlen parametrikus felület, a normálvektor ugyanaz mint a pozícióvektor
	
	feladatok: 
		- irány fényforrást pontra cserélni
		- fényeket textúrára alkalmazni
------------
|  		   |
|  6. hét  |
|		   |
------------

- új textúrakoordinátarendszer:
	-- origó bal alsó sarok

- mipmap (mintavételezési probléma)

- textúra mintavételező egységek (sampler)

- azért fragment shaderben olvassuk ki a textúrát (a vertexben gyorsabb lenne):
	-- pixelenként akarjuk kiértékelni, hogy azon a ponton épp milyen színnek kéne lennie
	
feladat:
	- körbefordulást meggátolni
	- balra - jobbra, előre - hátra mozgás W, A, S, D billentyűlenyomásokkal

------------
|  		   |
|  5. hét  |
|		   |
------------

- Ismétlés:
	01_Transforms:
		myVert.vert:
			- mátrixszorzás (jobb asszociatív)
		

- Több világtranszformáció alkalmazása: lehetséges
- gúlák egyenletes elosztása

- Indexbuffer

- Parametrikus felületek:
	- olyan egyenlet, amellyel bizonyos geometrikákat elő lehet állítani (henger, tórusz stb.)
	- másképpen: hogyan kellene egy darab papírból összehajtogatni?
	
		-- van egy parametrikus síkunk
		
		
		---------------------------------
		|	|	|	|	|	|	|	|	|
		|	|	|	|	|	|	|	|	|
		---------------------------------
		|	|	|	|	|	|	|	|	|
		|	|	|	|	|	|	|	|	|
		---------------------------------
		|	|	|	|	|	|	|	|	|
		|	|	|	|	|	|	|	|	|
		---------------------------------
		|	|	|	|	|	|	|	|	|
		|	|	|	|	|	|	|	|	|
		---------------------------------
		|	|	|	|	|	|	|	|	|
		|	|	|	|	|	|	|	|	|
		---------------------------------

------------
|  		   |
|  4. hét  |
|(itthoni) |
------------

(09-es videó)

Merőleges (ortogonális) vetítés helyett Perspektívikus:

- csonka gúla --> kamera néz valamerre

függőleges és vízszintes látószög (fokban)
van valamilyen első és hátsó vágósígja --> emiatt lesz csonkagúla




(08-as videó)

Feladat:
	A négyzeten az origó középpontú 0.5 sugarú környezetében háttérszínt lássunk,
	a többi helyen csak az előzőeket (Zoldberg fej, és szivárványszín).
	Csak fragmentekkel.

Feladat: pulzáljon

Feladat: írjunk mandelbrot halmazt fragment shaderben 
(mandelbrot: egy komplex számsíkon értelmezett halmaz, 
olyan c komplex számokból áll, melyekre ha megadunk egy rekurzív sorozatot hogy
 x1 = c, aztán x2 = x1^2 + c ...
 
 ez a sorozat a komplex szám hosszára tekintve korlátos, akkor beletesszük ebbe a halmazba (kirajzoljuk)

 

(06-os videó)

Dinamikus adatváltozóra átállás: array -> vector

Kör kirajzolása:

 - TriangleFan kirajzolásakor az első és utolsó pontnak meg kell egyeznie

Először nem látszódik semmi, mert a Z koordináta mindenütt 0:
	Z-buffering algoritmus:
		
		z-buffer           képernyő
		        /|                 /|                
               /||                /||                
              / ||               / ||                
             /| ||    	        /| ||                
            / | ||    	       / | ||                   
		   /| | ||    	      /| | ||                
		  / | | |/           / | | |/                
	     /| | | /           /| | | /                 
		/ | | |/           / | | |/                  
		| | | /   A   B    | | | /                   
		| | |/    /    |   | | |/                       (szem)
		|(0,2)-- /|-- /|---|x| /     <------------------- o
		| |/    /--  /-|   | |/                      
		| /    (0,4) (0,2) | /                       
		|/                 |/                        

		(z-bufferbe B háromszögnek a mélysége fog kerülni, kitakarja A háromszög egybeeső pontját a képernyőn)

	(Az algoritmus nem kisebbegyenlőt néz, hanem kisebbet!!!)
	
	megoldás:
	1. glEnable(GL_DEPTH_TEST) kikommentelése: érkezés szerint fog kirajzolódni
	2. glDepthFunc(GL_ALWAYS); => ilyen relációval vizsgálja meg a z értékeket (mindig a kisebb nyer), mindig a későbbi fog látszódni

------------
|  		   |
|  3. hét  |
|  		   |
------------

Grafikus-szerelőszalag:

	A videókártya, hogy dolgozza fel az adatokat.

	Feldolgozási lépések (cél, backbuffer celláiba adatot írni) - (pl. OpenGL 4.4 pipline):
	(Pozíció,szín) -> [...] -> millónyi pixel

		#1. Vertexeink feldolgozása (3D-s transzformációknál fontos): 
			
			(- modellezési transzformációval elhelyezhetjük a világkoordinátarendszerben               )
			(- ezután a kamera/nézeti transzformációval elhelyezhetjük a kamera koordinátarendszerben  )
			(- projekciós transzformáció révén átkerülhetünk kezdetben egy olyan koordinátarendszerben,) 
			(  ahol megtörténik a vágás/homogénosztás                                                  )
			(  ===> normalizált eszközkoordinátarendszer (kvázi egy kocka)                             )
			(  ===> képernyőkoordinátarendszer														   )
			
			(vertex shader)
			- [input] vertex -> [output] módosított vertex (cél #1.: végleges pozíció meghatározása)
			- 3D-s euklideszi koordinátarendszer
			
			- cél #2.: homogenizálás: videókártya jobban dolgozna homogén térbe ==> 

		#2. Vertexek geometriai feldolgozás:

			- glDrawArrays:
			
			Input => Output
			
			.    =>  /\ 
		   . .   => /__\

		
		#3. Raszterizáció és interpoláció:
			
			- primitív felbontása fragmentekre (pixel => pozíció, adott szín; fragment => pozíción lévő adathalmaz [később több adat lehet egy pozíción])
			
			- baricentrikus interpolálás

				Input ===> Output
					
					/\           o
				   /  \    ===> ooo
				  /    \       ooooo
				 /______\     ooooooo
				 
				 o információi lehetnek:
					-- textúra
					-- szín
					
				- fragmens shader:
					-- fragmensből pixelt gyárt: a fenti adatokból összeállít egy színt
				 

		#4. "Fragmens" feldolgozás:

			- merging stage:
				-- hogyan kombinálja a backbufferbe illetve a z pufferbe az értékeket (itt történik a z teszt)	
		 
		
			- minden fragmens színét megmondhatom, vagy eldobhatom
	 
			  CPU
			-------
			|  |  |
			|--|--|
			|__|__|
			   |
			   |           RAM
			   |----------------
			   |         |     |
			   |         |  v  |
			   |         |_____|
	   GPU 	   |
	---------  |
	|   | v |---
	--------- 
	
	GPU-ban több processzormag van: vertikális párhuzamosítás
	
	A szerelőszalag csoportokba osztja:
		- 200 db-ot x célra
		- 300 db-ot y célra
		- 500 db-ot z célra (pl. fragmentekre bontás)
		
		Egy idő után minden szalag fázisban lesz adat, amit feldolgozhat a szerelőszalag.
		

Programozható fázisok: Vertex és fragment feldolgozás

Alapértelmezett nVidia: 3-as (?)



------------
|  		   |
|  2. hét  |
|  		   |
------------

03_SDLOGL_FW:
	- SDL_GLContext --> OpenGL renderelő kontextus
	- többi kódba lett kommentelve
	
01_VAO_VBO:
	- lineárisan interpolálva vannak a számok
	
	kék 	------------------
	|		|				 |
	|		|				 |
	v		|				 |
			|				 |
			|				 |
			|				 |
	piros 	------------------

	- két háromszög: felső - alsó
		
		csúcspontokban eltárolni [P(osition), C(olor) párok]:
			- pozíció
			- szín
	
	- a többit oldja meg a videókártya
	
	Vertex-ekhez koordinátarendszer magyarázat: normalizált eszközkoordinátarendszer
	
	(-1,1)			^             (1,1)
					|
					|
					|
					|
					|
					|
(-1,0) -----------(0,0)-----------> (1,0)
					|
					|
					|
					|
					|
	(-1,-1)			|			  (1,-1)


	A projektben lévő háromszög:

	(-1,1)			^             (1,1)
	(kék)			|
		\			|
		|	\		|
		|		\	|
		|			| \
		|			|   \
(-1,0) -----------(0,0)--\-------> (1,0)
		|	<-^		|     \
		|  (*)|		|      \
		|	->|		|       \
		|			|		 \
		|-----------|---------\
	(piros)			|           \  (zöld)
	(-1,-1)			|			  (1,-1)



	GL_TRIANGLE_STRIP 	(mindig az újabb pontot hozzáköti a másik kettőhöz -> szalagszerű)
	GL_TRIANGLE_FAN 	(körszerű)

	TRIANGLE-s esetén figyelni kell a csúcspontok sorrendjére:
		- órajárással ellentétes sorrendben kell megadni őket (*)
		
	STRIP-nél az elsőt fogja követni
	
	hiba lehetőség:
		- rossz a pozíció
		- ki se rajzolja
		- rossz a körbejárási irány


	GPU/CPU/RAM:
	
		memória és a videókártya között kapcsolat --> régen 30 Gb/s (ez elég kevés)
		
		a videókártya annyira nagy, hogy rátesznek egy memóriát: GRAM
		
		GRAM: amikor töltődik a szoftver --> letöltjük a GRAM-ba az adatokat
		
		VBO (viszi az adatot)
		VertexArrayObject (magyarázza az adatot) --> videókártya nem tudja értelmezni, neki szimpla bináris (byte-ok)
		
		el kell magyarázni a videókártyának:
			struct Vertex {
				
			
			
			};
			
	házi:
		körlap középre - +1 pont
		külön VAO-VBO-ban legyen a körlap - +1 pont
		

=======
>>>>>>> 7b5bd420d42d8b66af819b038569ec77a63e0ab7
------------
|  		   |
|  1. hét  |
|  		   |
------------

- Projekt hierarchia: egy projekt - több solution
- Projekt megnyitása: solution file

- SDL: Cross-platform könyvtár
	-- Ad egy grafikus ablakot, és a platform-eseményeket lehetővé teszi.
	
	-- SDL.h:
		
01_Hello_SDL:

	#0. SDL_INIT_VIDEO: videós alrendszer

	#1. SDL_CreateWindow:
		- Képernyő-koordinátarendszer:
		
	0,0 ->
	|
	v
		-------------------------------
		|							  |
		|							  |
		|							  |
		|							  |
		|							  |
		|							  |
		|							  |
		-------------------------------
	
	#2. SDL_Renderer: rajzoló erőforrásunk
		- kell, hogy melyik képernyőre rajzolunk (előző lépésben készített ablak - SDL_CreateWindow)
	
	#3. SDL_SetRenderDrawColor: rajzolási szín
		- kell, hogy melyik renderelő használja
		
	#4. SDL_RenderClear(ren): képernyő letörlése + vonal rajzolása (már a renderelő koordinátarendszerében)
	
	Buffer-kezelés:
		- videókártya memóriájában van egy mátrix (van egy "kép"): háromkomponensű (byte) vektor:
			-- SDL_RenderDrawLine
			
		- párhuzamos rendszer: monitor vs. videókártya:
			-- frissítési frekvencia: pl. 60 Hz (másodpercenként 60-szor tudja frissíteni a képet)
		
		- backbuffer:
			-- monitor nem látja
			-- rajzolási utasítások ide érkeznek
			
		- frontbuffer:
			-- monitor látja
			
		=> SDL_RenderPresent (bufferek cserélése)
		
	Screentear:
		- nem a kép elejétől kezdek el olvasni
		- a monitor tudja jelezni, hogy most fejezte be a kép rajzolását/olvasását (VSYNC)
		- SDL_RENDERER_PRESENTVSYNC - addig nem cserél a videókártya, amíg a monitor nem végzett
		
	10 szög rajzolása:
	
	for( int i = 0; i < 10; ++i ) {
		SDL_SetRenderDrawLine(
			ren,
			cos(2*M_PI / 10 * i) * 100 + 200,
			sin(2*M_PI / 10 * i) * 100 + 200,
			
			cos(2*M_PI / 10 * (i+1)) * 100 + 200,
			sin(2*M_PI / 10 * (i+1)) * 100 + 200
		);
	}

02_SDLEvents:
	- Üzenetfeldolgozás:
		-- INIT -> <<steps>> -> CLEAR
		
		<<steps>>:
			1. input: 	megnézzük, hogy az előző képkocka óta milyen inputok voltak (operációs rendszer folyton gyűjti az inputokat)
						
				- SDL_PollEvent(&ev):
					-- legrégebben történt eseményt veszi ki az operációs rendszer által gyűjtött "listájából"
					
			2. update: 	az alkalmazásunk belső állapotát (memóriáját) frissítjük:
			
				- SDL_RenderClear
			
			3. render:	kirajzolom az új állapotot
			
		Amikor vége: render-ből -> CLEAR
		
		-- általában az update és a render összemosódik sajnos
		
Számítógépes grafika - 2019/20 - Tavaszi félév

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
	

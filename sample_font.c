	TTF_Font *font = TTF_OpenFont("./assets/fonts/RobotoMono_Regular.ttf", 100);
	if (!font) {
		printf("font failed: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Color color = { 255, 0, 0, 255 };
	SDL_Surface *surface = TTF_RenderText_Solid(font, "SHIT_TEXT", 9, color);
	if (!surface) {
		printf("surface failed: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surface);
	if (!text) {
		printf("text failed: %s\n", SDL_GetError());
		return 1;
	}
	SDL_DestroySurface(surface);
	SDL_FRect rect = { 100, 100, 500, 100 };
	SDL_RenderTexture(renderer, text, NULL, &rect);
	SDL_DestroyTexture(text);
	SDL_RenderPresent(renderer);


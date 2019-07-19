#ifndef _TILEMAP
#define _TILEMAP

class Tile //: public Grid, public Image
{
public:
    Tile();
	~Tile();
    void add(const Image& tile, int id); // image_list.push_back(std::make_pair(&const_cast<Image&>(tile), id));
private:
    std::vector<std::pair<Image *, int>> image_list; // image_list[i].first or image_list[i].second
};
#endif
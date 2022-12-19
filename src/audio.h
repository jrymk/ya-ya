#pragma once
#ifndef YAYA_AUDIO_H
#define YAYA_AUDIO_H

#include <SFML/Audio.hpp>
#include <string>
#include <utility>
#include <iterator>
#include <vector>

#define BGM_ID 2

class Audio {
private :
    int samples = 0;
    sf::SoundBuffer buffer;
    std::vector<sf::SoundBuffer> bufferVec;
    sf::Sound sound;
    std::vector<sf::Sound> soundVec;
    std::vector<bool> loop;
public :
    inline Audio() = default;

    inline ~Audio() = default;

    void loadSound(std::string fileName);

    void playSound(int id);

    void playBGM();

    void muteSound();

    void setLoop(int id, bool set);

    void setVolume(int vol);
};

template <typename T>
struct iterator_extractor {
    typedef typename T::iterator type;
};

template <typename T>
struct iterator_extractor<T const> {
    typedef typename T::const_iterator type;
};

template <typename T>
class Indexer {
private:
    T& _container;
public:
    class iterator {
        typedef typename iterator_extractor<T>::type inner_iterator;

        typedef typename std::iterator_traits<inner_iterator>::reference inner_reference;
     private:
        size_t _pos;
        inner_iterator _it;
    public:
        typedef std::pair<size_t, inner_reference> reference;

        iterator(inner_iterator it): _pos(0), _it(it) {}

        reference operator*() const { return reference(_pos, *_it); }

        iterator& operator++() { ++_pos; ++_it; return *this; }
        iterator operator++(int) { iterator tmp(*this); ++*this; return tmp; }

        bool operator==(iterator const& it) const { return _it == it._it; }
        bool operator!=(iterator const& it) const { return !(*this == it); }
    };

    Indexer(T& t): _container(t) {}

    iterator begin() const { return iterator(_container.begin()); }
    iterator end() const { return iterator(_container.end()); }
};

template <typename T>
Indexer<T> index(T& t) { return Indexer<T>(t); }

#endif
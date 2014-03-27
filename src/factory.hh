template<>
NoteInterface* NewNote<Note>(int key = 0, int velocity = 0) {
  return new Note(int key, int velocity);
}

template<>
NoteInterface* NewNote<MockNote>(int key = 0, int velocity = 0) {
  return new MockNote();
}

/**
 * The real deal
 */
template<>
TrackEntryInterface* NewTrackEntry<TrackEntry,Notes,Effects>
(TrackInterface* track,
 Notes* notes = new Notes(),
 Effects* effects = new Effects())
{
  ...
  Note* note = NewNote<Note>();
  notes->push_back(note);
  TrackEntry* track_entry = new TrackEntry(notes, effects, track);
  ...
  return track entry;
}

/**
 * The real deal, but with mocked contents
 */
template<>
TrackEntryInterface* NewTrackEntry<TrackEntry,MockNotes,MockEffects>
(TrackInterface* track,
 MockNotes* notes = new MockNotes(),
 MockEffects* effects = new MockEffects())
{
  ...
  MockNote* note = NewNote<MockNote>();
  notes->push_back(note);
  TrackEntry* track_entry = new TrackEntry(notes, effects, track);
  ...
  return track entry;
}

/**
 * The complete mock.
 */
template<>
TrackEntryInterface* NewTrackEntry<MockTrackEntry,MockNotes,MockEffects>
(TrackInterface* track, MockNotes* notes = NULL, MockEffects* effects = NULL)
{
  ...
  MockTrackEntry* track_entry = new MockTrackEntry();
  ON_CALL(*track_entry, get_track()).WillByDefault(Return(track));
  ON_CALL(*track_entry, get_notes()).WillByDefault(Return(notes));
  ON_CALL(*track_entry, get_effects()).WillByDefault(Return(effects));
  ...
  return track entry;
}

TrackEntryInterface*(*new_track_entry)

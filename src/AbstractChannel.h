/**
 * @AbstractChannel.h
 *
 * Pure virtual class interface for representing a channel.
 */

class AbstractNoteHeader {

protected:

  string *name;
  string *short_name;
  bool solo;
  bool mute;

};

class AbstractParameterHeader {

protected:

  string *name;
  string *short_name;

};

class AbstractChannel {

protected:

  string *name;
  AbstractNoteHeaders *note_headers;
  AbstractParameterHeaders *parameter_headers;
  bool solo;
  bool mute;

public:

  AbstractChannel(NoteHeaders *note_headers, ParameterHeaders *parameter_headers);



};

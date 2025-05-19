# 1.6.2.25.05.19
- Add `window.IsDark` boolean property. The `on_colours_changed` callback has always
responded to dark mode being toggled.
- Ensure `fb.IsMainMenuCommandChecked` is always reliable.

# 1.6.2.25.05.14
- Add `fb.AddLocationsAsync(locations)` and `on_locations_added` callback.

`locations` must be an array of strings and it can contain file paths, playlists or urls.

Example:

```js
function on_mouse_lbtn_dblclk() {
	var files = ["z:\\1.mp3", "z:\\2.flac"];
	var task_id = fb.AddLocationsAsync(files);
	console.log("got task_id", task_id);
}

function on_locations_added(task_id, handle_list) {
	console.log("callback task_id", task_id);
	console.log(handle_list.Count);
}
```

# 1.6.2.25.05.13
- Add `FbMetadbHandleList` `SaveAs`. Saves using native `.fpl`
format so you should use that as the file extension. The
parent folder must already exist.

- Add `fb.ShowPictureViewer(image_path)`. This uses the image
viewer built in to `foobar2000`.

# 1.6.2.25.05.10
- `foobar2000` `2.25` preview has changed the behaviour of `FbMetadbHandle` `RawPath` if you
have a portable install and music files on the same drive. Any code that checks `startsWith("file://")`
will fail because the `RawPath` now starts with `file-relative://`. This release restores the old
behaviour.

# 1.6.2.25.05.05

- Requires at least `foobar2000` `2.0` because new methods from the `SDK` are being used.

- Add `fb.GetAudioChunk` / `FbAudioChunk` interface. See `vu meter` sample.

- Add `utils.GetClipboardText` / `utils.SetClipboardText`.

- Add `plman.GetGUID` / `plman.FindByGUID`.

A `GUID` is a unique identifier that persists with the playlist for its entire
lifetime between restarts and being renamed.

```js
var guid_str = plman.GetGUID(playlistIndex); // throws an error if playlistIndex is out of bounds
var playlistIndex = plman.FindByGUID(guid_str); // returns playlistIndex or -1 if not found
```

- Fix `utils.ColourPicker` bugs

- Fix `utils.IsFile` / `utils.IsDirectory` bugs.

- Update various samples fixed by `regor`.

- The `on_library_items_changed` callback now has a secondary `fromhook` argument so you can ignore updates that are not tag edits but database updates from components like `foo_playcount`.

```js
function on_library_items_changed(handles, fromhook) {
    if (fromhook)
        return;
   // react to actual file tag changes here
}
```

- Various improvements to handle list iteration methods.

- Various other bug fixes.

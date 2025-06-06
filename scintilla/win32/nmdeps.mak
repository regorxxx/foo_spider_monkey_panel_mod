# Created by DepGen.py. To recreate, run DepGen.py.
$(DIR_O)/HanjaDic.obj: \
	HanjaDic.cxx \
	../src/UniConversion.h \
	HanjaDic.h
$(DIR_O)/PlatWin.obj: \
	PlatWin.cxx \
	../include/Platform.h \
	../src/XPM.h \
	../src/UniConversion.h \
	../src/DBCS.h \
	../src/FontQuality.h \
	PlatWin.h
$(DIR_O)/ScintillaDLL.obj: \
	ScintillaDLL.cxx \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	ScintillaWin.h
$(DIR_O)/ScintillaWin.obj: \
	ScintillaWin.cxx \
	../include/Platform.h \
	../include/ILoader.h \
	../include/Sci_Position.h \
	../include/ILexer.h \
	../include/Scintilla.h \
	../lexlib/CharacterCategory.h \
	../src/Position.h \
	../src/UniqueString.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/ContractionState.h \
	../src/CellBuffer.h \
	../src/CallTip.h \
	../src/KeyMap.h \
	../src/Indicator.h \
	../src/LineMarker.h \
	../src/Style.h \
	../src/ViewStyle.h \
	../src/CharClassify.h \
	../src/Decoration.h \
	../src/CaseFolder.h \
	../src/Document.h \
	../src/CaseConvert.h \
	../src/UniConversion.h \
	../src/Selection.h \
	../src/PositionCache.h \
	../src/EditModel.h \
	../src/MarginView.h \
	../src/EditView.h \
	../src/Editor.h \
	../src/ElapsedPeriod.h \
	../src/AutoComplete.h \
	../src/ScintillaBase.h \
	PlatWin.h \
	HanjaDic.h \
	ScintillaWin.h
$(DIR_O)/AutoComplete.obj: \
	../src/AutoComplete.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../lexlib/CharacterSet.h \
	../src/Position.h \
	../src/AutoComplete.h
$(DIR_O)/CallTip.obj: \
	../src/CallTip.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/Position.h \
	../src/IntegerRectangle.h \
	../src/CallTip.h
$(DIR_O)/CaseConvert.obj: \
	../src/CaseConvert.cxx \
	../src/CaseConvert.h \
	../src/UniConversion.h
$(DIR_O)/CaseFolder.obj: \
	../src/CaseFolder.cxx \
	../src/CaseFolder.h \
	../src/CaseConvert.h
$(DIR_O)/Catalogue.obj: \
	../src/Catalogue.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/LexerModule.h \
	../lexlib/CatalogueModules.h \
	../src/Catalogue.h
$(DIR_O)/CatalogueL.obj: \
	../src/Catalogue.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/LexerModule.h \
	../lexlib/CatalogueModules.h \
	../src/Catalogue.h
$(DIR_O)/CellBuffer.obj: \
	../src/CellBuffer.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/Position.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/CellBuffer.h \
	../src/UniConversion.h
$(DIR_O)/CharClassify.obj: \
	../src/CharClassify.cxx \
	../lexlib/CharacterSet.h \
	../src/CharClassify.h
$(DIR_O)/ContractionState.obj: \
	../src/ContractionState.cxx \
	../include/Platform.h \
	../src/Position.h \
	../src/UniqueString.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/SparseVector.h \
	../src/ContractionState.h
$(DIR_O)/DBCS.obj: \
	../src/DBCS.cxx \
	../src/DBCS.h
$(DIR_O)/Decoration.obj: \
	../src/Decoration.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/Position.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/Decoration.h
$(DIR_O)/Document.obj: \
	../src/Document.cxx \
	../include/Platform.h \
	../include/ILoader.h \
	../include/Sci_Position.h \
	../include/ILexer.h \
	../include/Scintilla.h \
	../lexlib/CharacterSet.h \
	../lexlib/CharacterCategory.h \
	../src/Position.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/CellBuffer.h \
	../src/PerLine.h \
	../src/CharClassify.h \
	../src/Decoration.h \
	../src/CaseFolder.h \
	../src/Document.h \
	../src/RESearch.h \
	../src/UniConversion.h \
	../src/ElapsedPeriod.h
$(DIR_O)/EditModel.obj: \
	../src/EditModel.cxx \
	../include/Platform.h \
	../include/ILoader.h \
	../include/Sci_Position.h \
	../include/ILexer.h \
	../include/Scintilla.h \
	../lexlib/CharacterCategory.h \
	../src/Position.h \
	../src/UniqueString.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/ContractionState.h \
	../src/CellBuffer.h \
	../src/KeyMap.h \
	../src/Indicator.h \
	../src/LineMarker.h \
	../src/Style.h \
	../src/ViewStyle.h \
	../src/CharClassify.h \
	../src/Decoration.h \
	../src/CaseFolder.h \
	../src/Document.h \
	../src/UniConversion.h \
	../src/Selection.h \
	../src/PositionCache.h \
	../src/EditModel.h
$(DIR_O)/Editor.obj: \
	../src/Editor.cxx \
	../include/Platform.h \
	../include/ILoader.h \
	../include/Sci_Position.h \
	../include/ILexer.h \
	../include/Scintilla.h \
	../lexlib/CharacterSet.h \
	../lexlib/CharacterCategory.h \
	../src/Position.h \
	../src/UniqueString.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/ContractionState.h \
	../src/CellBuffer.h \
	../src/PerLine.h \
	../src/KeyMap.h \
	../src/Indicator.h \
	../src/LineMarker.h \
	../src/Style.h \
	../src/ViewStyle.h \
	../src/CharClassify.h \
	../src/Decoration.h \
	../src/CaseFolder.h \
	../src/Document.h \
	../src/UniConversion.h \
	../src/Selection.h \
	../src/PositionCache.h \
	../src/EditModel.h \
	../src/MarginView.h \
	../src/EditView.h \
	../src/Editor.h \
	../src/ElapsedPeriod.h
$(DIR_O)/EditView.obj: \
	../src/EditView.cxx \
	../include/Platform.h \
	../include/ILoader.h \
	../include/Sci_Position.h \
	../include/ILexer.h \
	../include/Scintilla.h \
	../lexlib/CharacterSet.h \
	../lexlib/CharacterCategory.h \
	../src/Position.h \
	../src/IntegerRectangle.h \
	../src/UniqueString.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/ContractionState.h \
	../src/CellBuffer.h \
	../src/PerLine.h \
	../src/KeyMap.h \
	../src/Indicator.h \
	../src/LineMarker.h \
	../src/Style.h \
	../src/ViewStyle.h \
	../src/CharClassify.h \
	../src/Decoration.h \
	../src/CaseFolder.h \
	../src/Document.h \
	../src/UniConversion.h \
	../src/Selection.h \
	../src/PositionCache.h \
	../src/EditModel.h \
	../src/MarginView.h \
	../src/EditView.h \
	../src/ElapsedPeriod.h
$(DIR_O)/ExternalLexer.obj: \
	../src/ExternalLexer.cxx \
	../include/Platform.h \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/LexerModule.h \
	../src/Catalogue.h \
	../src/ExternalLexer.h
$(DIR_O)/Indicator.obj: \
	../src/Indicator.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/IntegerRectangle.h \
	../src/Indicator.h \
	../src/XPM.h
$(DIR_O)/KeyMap.obj: \
	../src/KeyMap.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/KeyMap.h
$(DIR_O)/LineMarker.obj: \
	../src/LineMarker.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/IntegerRectangle.h \
	../src/XPM.h \
	../src/LineMarker.h
$(DIR_O)/MarginView.obj: \
	../src/MarginView.cxx \
	../include/Platform.h \
	../include/ILoader.h \
	../include/Sci_Position.h \
	../include/ILexer.h \
	../include/Scintilla.h \
	../lexlib/CharacterCategory.h \
	../src/Position.h \
	../src/IntegerRectangle.h \
	../src/UniqueString.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/ContractionState.h \
	../src/CellBuffer.h \
	../src/KeyMap.h \
	../src/Indicator.h \
	../src/LineMarker.h \
	../src/Style.h \
	../src/ViewStyle.h \
	../src/CharClassify.h \
	../src/Decoration.h \
	../src/CaseFolder.h \
	../src/Document.h \
	../src/UniConversion.h \
	../src/Selection.h \
	../src/PositionCache.h \
	../src/EditModel.h \
	../src/MarginView.h \
	../src/EditView.h
$(DIR_O)/PerLine.obj: \
	../src/PerLine.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/Position.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/CellBuffer.h \
	../src/PerLine.h
$(DIR_O)/PositionCache.obj: \
	../src/PositionCache.cxx \
	../include/Platform.h \
	../include/ILoader.h \
	../include/Sci_Position.h \
	../include/ILexer.h \
	../include/Scintilla.h \
	../lexlib/CharacterCategory.h \
	../src/Position.h \
	../src/UniqueString.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/ContractionState.h \
	../src/CellBuffer.h \
	../src/KeyMap.h \
	../src/Indicator.h \
	../src/LineMarker.h \
	../src/Style.h \
	../src/ViewStyle.h \
	../src/CharClassify.h \
	../src/Decoration.h \
	../src/CaseFolder.h \
	../src/Document.h \
	../src/UniConversion.h \
	../src/Selection.h \
	../src/PositionCache.h
$(DIR_O)/RESearch.obj: \
	../src/RESearch.cxx \
	../src/Position.h \
	../src/CharClassify.h \
	../src/RESearch.h
$(DIR_O)/RunStyles.obj: \
	../src/RunStyles.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/Position.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h
$(DIR_O)/ScintillaBase.obj: \
	../src/ScintillaBase.cxx \
	../include/Platform.h \
	../include/ILoader.h \
	../include/Sci_Position.h \
	../include/ILexer.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/PropSetSimple.h \
	../lexlib/CharacterCategory.h \
	../lexlib/LexerModule.h \
	../src/Catalogue.h \
	../src/Position.h \
	../src/UniqueString.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/ContractionState.h \
	../src/CellBuffer.h \
	../src/CallTip.h \
	../src/KeyMap.h \
	../src/Indicator.h \
	../src/LineMarker.h \
	../src/Style.h \
	../src/ViewStyle.h \
	../src/CharClassify.h \
	../src/Decoration.h \
	../src/CaseFolder.h \
	../src/Document.h \
	../src/Selection.h \
	../src/PositionCache.h \
	../src/EditModel.h \
	../src/MarginView.h \
	../src/EditView.h \
	../src/Editor.h \
	../src/AutoComplete.h \
	../src/ScintillaBase.h \
	../src/ExternalLexer.h
$(DIR_O)/ScintillaBaseL.obj: \
	../src/ScintillaBase.cxx \
	../include/Platform.h \
	../include/ILoader.h \
	../include/Sci_Position.h \
	../include/ILexer.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/PropSetSimple.h \
	../lexlib/CharacterCategory.h \
	../lexlib/LexerModule.h \
	../src/Catalogue.h \
	../src/Position.h \
	../src/UniqueString.h \
	../src/SplitVector.h \
	../src/Partitioning.h \
	../src/RunStyles.h \
	../src/ContractionState.h \
	../src/CellBuffer.h \
	../src/CallTip.h \
	../src/KeyMap.h \
	../src/Indicator.h \
	../src/LineMarker.h \
	../src/Style.h \
	../src/ViewStyle.h \
	../src/CharClassify.h \
	../src/Decoration.h \
	../src/CaseFolder.h \
	../src/Document.h \
	../src/Selection.h \
	../src/PositionCache.h \
	../src/EditModel.h \
	../src/MarginView.h \
	../src/EditView.h \
	../src/Editor.h \
	../src/AutoComplete.h \
	../src/ScintillaBase.h \
	../src/ExternalLexer.h
$(DIR_O)/Selection.obj: \
	../src/Selection.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/Position.h \
	../src/Selection.h
$(DIR_O)/Style.obj: \
	../src/Style.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/Style.h
$(DIR_O)/UniConversion.obj: \
	../src/UniConversion.cxx \
	../src/UniConversion.h
$(DIR_O)/UniqueString.obj: \
	../src/UniqueString.cxx \
	../src/UniqueString.h
$(DIR_O)/ViewStyle.obj: \
	../src/ViewStyle.cxx \
	../include/Platform.h \
	../include/Scintilla.h \
	../include/Sci_Position.h \
	../src/Position.h \
	../src/UniqueString.h \
	../src/Indicator.h \
	../src/XPM.h \
	../src/LineMarker.h \
	../src/Style.h \
	../src/ViewStyle.h
$(DIR_O)/XPM.obj: \
	../src/XPM.cxx \
	../include/Platform.h \
	../src/XPM.h
$(DIR_O)/Accessor.obj: \
	../lexlib/Accessor.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/PropSetSimple.h \
	../lexlib/WordList.h \
	../lexlib/LexAccessor.h \
	../lexlib/Accessor.h
$(DIR_O)/CharacterCategory.obj: \
	../lexlib/CharacterCategory.cxx \
	../lexlib/CharacterCategory.h
$(DIR_O)/CharacterSet.obj: \
	../lexlib/CharacterSet.cxx \
	../lexlib/CharacterSet.h
$(DIR_O)/DefaultLexer.obj: \
	../lexlib/DefaultLexer.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/PropSetSimple.h \
	../lexlib/WordList.h \
	../lexlib/LexAccessor.h \
	../lexlib/Accessor.h \
	../lexlib/LexerModule.h \
	../lexlib/DefaultLexer.h
$(DIR_O)/LexerBase.obj: \
	../lexlib/LexerBase.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/PropSetSimple.h \
	../lexlib/WordList.h \
	../lexlib/LexAccessor.h \
	../lexlib/Accessor.h \
	../lexlib/LexerModule.h \
	../lexlib/LexerBase.h
$(DIR_O)/LexerModule.obj: \
	../lexlib/LexerModule.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/PropSetSimple.h \
	../lexlib/WordList.h \
	../lexlib/LexAccessor.h \
	../lexlib/Accessor.h \
	../lexlib/LexerModule.h \
	../lexlib/LexerBase.h \
	../lexlib/LexerSimple.h
$(DIR_O)/LexerNoExceptions.obj: \
	../lexlib/LexerNoExceptions.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/PropSetSimple.h \
	../lexlib/WordList.h \
	../lexlib/LexAccessor.h \
	../lexlib/Accessor.h \
	../lexlib/LexerModule.h \
	../lexlib/LexerBase.h \
	../lexlib/LexerNoExceptions.h
$(DIR_O)/LexerSimple.obj: \
	../lexlib/LexerSimple.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/PropSetSimple.h \
	../lexlib/WordList.h \
	../lexlib/LexAccessor.h \
	../lexlib/Accessor.h \
	../lexlib/LexerModule.h \
	../lexlib/LexerBase.h \
	../lexlib/LexerSimple.h
$(DIR_O)/PropSetSimple.obj: \
	../lexlib/PropSetSimple.cxx \
	../lexlib/PropSetSimple.h
$(DIR_O)/StyleContext.obj: \
	../lexlib/StyleContext.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../lexlib/LexAccessor.h \
	../lexlib/Accessor.h \
	../lexlib/StyleContext.h \
	../lexlib/CharacterSet.h
$(DIR_O)/WordList.obj: \
	../lexlib/WordList.cxx \
	../lexlib/WordList.h
$(DIR_O)/LexCPP.obj: \
	../lexers/LexCPP.cxx \
	../include/ILexer.h \
	../include/Sci_Position.h \
	../include/Scintilla.h \
	../include/SciLexer.h \
	../lexlib/StringCopy.h \
	../lexlib/WordList.h \
	../lexlib/LexAccessor.h \
	../lexlib/Accessor.h \
	../lexlib/StyleContext.h \
	../lexlib/CharacterSet.h \
	../lexlib/LexerModule.h \
	../lexlib/OptionSet.h \
	../lexlib/SparseState.h \
	../lexlib/SubStyles.h

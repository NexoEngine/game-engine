# Translation and Mapping: French Requirements to GitHub Issues

This document provides the translation and mapping from the original French feature requirements to the created GitHub issues for the Advanced Game Engine milestone.

## Original French Requirements

The following features were requested in French for the "Maj éditeur" (Editor Update):

### Original French List:
1. Paramètre import asset (pouvoir modifier les parametres dun asset pendant son import)
2. Preview miniature models (dans le front de lasset manager)
3. Clic droit assets manager (metadata, propriétés de l'asset, supprimer, renommer, ajouter un dossier, etc)
4. Ajout component (de manière générale dans le front pouvoir ajouter un component via un bouton dans linspecteur d'une entité)
5. Inspecteur du component physique (rigid body)
6. Pouvoir créer nos propres colliders
7. Finir la game scene (partie éditeur faites mais choses a revoir notamment sur la toolbar) et faire en sorte que ça soit une vrai scene editeur (désac scripting é physique notamment) (en gros pouvoir lancer le jeu en mode simulation depuis lediteur)
8. Search bar dans l'assets manager
9. Vérifier que toutes les actions faites par l'utilisateur soient inclues dans le undo redo
10. passage script + shader en assets
11. checker rtoutes les issues sonarcloud
12. augmenter coverage
13. ajouter tests manuels de l'édtieur dans les fichiers de tests

---

## Translation and Issue Mapping

### 1. Asset Import Parameters
**French**: Paramètre import asset (pouvoir modifier les parametres dun asset pendant son import)  
**English**: Asset import parameters (ability to modify asset parameters during import)  
**GitHub Issue**: `feat: configurable asset import parameters`  
**Priority**: High  
**Category**: Editor/Assets  

### 2. Model Thumbnail Previews  
**French**: Preview miniature models (dans le front de lasset manager)  
**English**: Model thumbnail previews (in the asset manager frontend)  
**GitHub Issue**: `feat: model thumbnail previews in asset manager`  
**Priority**: Medium  
**Category**: Editor/UI  

### 3. Right-Click Asset Manager
**French**: Clic droit assets manager (metadata, propriétés de l'asset, supprimer, renommer, ajouter un dossier, etc)  
**English**: Right-click asset manager (metadata, asset properties, delete, rename, add folder, etc)  
**GitHub Issue**: `feat: asset manager right-click context menu`  
**Priority**: High  
**Category**: Editor/Assets  

### 4. Add Component Button
**French**: Ajout component (de manière générale dans le front pouvoir ajouter un component via un bouton dans linspecteur d'une entité)  
**English**: Add component (generally in the frontend, ability to add a component via a button in the entity inspector)  
**GitHub Issue**: `feat: add component button in entity inspector`  
**Priority**: High  
**Category**: Editor/ECS  

### 5. Physics Component Inspector
**French**: Inspecteur du component physique (rigid body)  
**English**: Physics component inspector (rigid body)  
**GitHub Issue**: `feat: physics component inspector for rigid bodies`  
**Priority**: High  
**Category**: Editor/Physics  

### 6. Custom Collider Creation
**French**: Pouvoir créer nos propres colliders  
**English**: Ability to create our own colliders  
**GitHub Issue**: `feat: custom collider creation and editing`  
**Priority**: Medium  
**Category**: Engine/Physics  

### 7. Complete Game Scene
**French**: Finir la game scene (partie éditeur faites mais choses a revoir notamment sur la toolbar) et faire en sorte que ça soit une vrai scene editeur (désac scripting é physique notamment) (en gros pouvoir lancer le jeu en mode simulation depuis lediteur)  
**English**: Finish the game scene (editor part done but things to review, especially the toolbar) and make it a real scene editor (disable scripting and physics notably) (basically run the game in simulation mode from the editor)  
**GitHub Issue**: `feat: complete game scene with simulation mode`  
**Priority**: High  
**Category**: Editor/Simulation  

### 8. Asset Manager Search Bar
**French**: Search bar dans l'assets manager  
**English**: Search bar in the assets manager  
**GitHub Issue**: `feat: search functionality in asset manager`  
**Priority**: Medium  
**Category**: Editor/Assets  

### 9. Comprehensive Undo/Redo
**French**: Vérifier que toutes les actions faites par l'utilisateur soient inclues dans le undo redo  
**English**: Ensure that all user actions are included in undo/redo  
**GitHub Issue**: `feat: comprehensive undo/redo for all user actions`  
**Priority**: High  
**Category**: Editor/Core  

### 10. Scripts and Shaders as Assets
**French**: passage script + shader en assets  
**English**: Convert scripts + shaders to assets  
**GitHub Issue**: `feat: convert scripts and shaders to asset system`  
**Priority**: Medium  
**Category**: Engine/Assets  

### 11. SonarCloud Issues
**French**: checker rtoutes les issues sonarcloud  
**English**: Check all SonarCloud issues  
**GitHub Issue**: `fix: resolve all SonarCloud code quality issues`  
**Priority**: Medium  
**Category**: CI/CD/Quality  

### 12. Increase Coverage
**French**: augmenter coverage  
**English**: Increase coverage  
**GitHub Issue**: `feat: increase test coverage across the codebase`  
**Priority**: Medium  
**Category**: Testing/Quality  

### 13. Manual Editor Tests
**French**: ajouter tests manuels de l'édtieur dans les fichiers de tests  
**English**: Add manual editor tests to test files  
**GitHub Issue**: `feat: add manual editor testing procedures`  
**Priority**: Low  
**Category**: Testing/Documentation  

---

## Summary Statistics

- **Total Issues**: 13
- **High Priority**: 6 issues (46%)
- **Medium Priority**: 6 issues (46%)
- **Low Priority**: 1 issue (8%)

### By Category:
- **Editor**: 8 issues (62%)
- **Engine**: 2 issues (15%)
- **Testing**: 2 issues (15%)
- **CI/CD**: 1 issue (8%)

### By Size Estimate:
- **X-Large**: 1 issue
- **Large**: 6 issues
- **Medium**: 5 issues
- **Small**: 1 issue

---

## Implementation Notes

1. **French Context**: All requirements focus on editor improvements ("Maj éditeur" = Editor Update)
2. **User Experience Focus**: Most features aim to improve user workflow and editor usability
3. **Asset Management**: Heavy emphasis on asset-related features (5 out of 13 issues)
4. **Quality Assurance**: 3 issues dedicated to testing and code quality
5. **Physics Integration**: 2 issues specifically for physics system integration

## Dependencies and Relationships

- Issues 1, 2, 8 are all asset manager related and can be developed in parallel
- Issue 3 (right-click menu) depends on having basic asset operations
- Issues 5 and 6 are physics-related and should be coordinated
- Issue 7 (game scene) is complex and may depend on multiple other systems
- Issue 9 (undo/redo) affects all other editor features
- Issues 11-13 are quality/testing related and can be ongoing parallel work

This comprehensive mapping ensures that all original French requirements are properly translated, categorized, and planned for implementation in the Advanced Game Engine milestone.
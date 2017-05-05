<?php
/**
 * @file
 * paragraphs-item--holiday-search-tiles.tpl.php
 */
?>

<div class="paragraphs-item paragraphs-item--holiday-search-tiles entity-paragraphs-item">
  <div class="container">
    <div class="row">
      <div class="holiday-search-tiles">
        <?php foreach (element_children($holiday_search_tiles) as $holiday_search_tile): ?>
          <?php print render($holiday_search_tiles[$holiday_search_tile]) ?>
        <?php endforeach ?>
      </div>
    </div>

    <div class="row">
      <?php if (!empty($more_results_link_text) && !empty($more_results_link_url)): ?>
        <div class="more-results-link">
          <a href="<?php print $more_results_link_url ?>"><?php print $more_results_link_text ?></a>
        </div>
      <?php endif ?>
    </div>
  </div>
</div>

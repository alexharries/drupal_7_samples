<?php
/**
 * @file
 * paragraphs-item--holiday-search-tile.tpl.php
 */

?>

<div class="paragraphs-item--holiday-search-tiles--holiday-search-tile">
  <?php if (!empty($holiday_search_tile_not_found)): ?>
    <!-- Holiday not found :( -->
  <?php else: ?>
    <a href="<?php print $holiday_url ?>" class="holiday-tile">
      <?php if (!empty($image)): ?>
        <!-- Image -->
        <div class="image">
          <img src="<?php print $image ?>" alt="<?php print $image_alt ?>" title="<?php print $image_title ?>"/>
        </div>
      <?php else: ?>
        <!-- No image -->
        <div class="no-image"></div>
      <?php endif ?>

      <div class="text">
        <?php if (!empty($place_name)): ?>
          <!-- Place name -->
          <h3 class="place-name"><?php print $place_name ?></h3>
        <?php endif ?>

        <?php if (!empty($holiday_name)): ?>
          <!-- Holiday name -->
          <p class="holiday-name"><?php print $holiday_name ?>&nbsp;&raquo;</p>
        <?php endif ?>

        <?php if (!empty($product_tag)): ?>
          <!-- Product tag -->
          <p class="product-tag">
            <span class="product-tag-inside product-tag-type--<?php print $product_tag_class ?>"><?php print $product_tag ?></span>
          </p>
        <?php endif ?>

        <?php if (!empty($days_from)): ?>
          <!-- Days from -->
          <p class="days-from"><?php print $days_from ?></p>
        <?php endif ?>

        <?php if (!empty($price)): ?>
          <!-- Price -->
          <p class="price">
            <?php
            // Note that we have preprocessed this field and run it through t()
            // including its spans; this allows translators to put place the currency
            // and "pp" text on the side appropriate to the language they're
            // translating into.
            ?>
            <?php print $price ?>
          </p>
        <?php endif ?>
      </div>
    </a>
  <?php endif ?>
</div>
